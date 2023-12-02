#include "pch.h"
#include "Shader.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLShader.h"
#include "Core/Render/Texture.h"
#include "Core/Render/Texture2D.h"
#include "Core/Render/Cubemap.h"
#include "Core/Render/RenderCommand.h"


namespace PR
{
	std::unordered_map<std::string, PropertyValue> Shader::s_PropertyValue;
	Shader* Shader::s_PreShader = nullptr;

	std::shared_ptr<Shader> Shader::Create(const std::string& filepath)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
			{
				PR_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(filepath);
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:
			{
				PR_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void Shader::UploadProperty(const std::unordered_map<std::string, PropertyValue>& materialValue)
	{
		int textureSlot = 0;
		int imageSlot = 0;
		int bufferBindingPoint = 0;
		for (auto& p : m_PropertyData)
		{
			std::any value;
			if (materialValue.find(p.Name) != materialValue.end() && materialValue.at(p.Name).Type == p.Type)
			{
				value = materialValue.at(p.Name).Value;
			}
			else if (s_PropertyValue.find(p.Name) != s_PropertyValue.end() && s_PropertyValue[p.Name].Type == p.Type)
			{
				value = s_PropertyValue[p.Name].Value;
			}
			else
			{
				if (p.Type == PropertyType::Texture)
				{
					Texture* defaultTexture = Texture2D::GetWhiteTexture().get();
					value = defaultTexture;
				}
			}

			if (value.has_value())
			{
				switch (p.Type)
				{
					case PropertyType::Int:
					{
						UploadInt(p.Name, std::any_cast<int>(value));
						break;							
					}
					case PropertyType::Float:
					{
						UploadFloat(p.Name, std::any_cast<float>(value));
						break;
					}
					case PropertyType::Float2:
					{
						UploadFloat2(p.Name, std::any_cast<glm::vec2>(value));
						break;
					}
					case PropertyType::Float3:
					{
						UploadFloat3(p.Name, std::any_cast<glm::vec3>(value));
						break;
					}
					case PropertyType::Float4:
					{
						UploadFloat4(p.Name, std::any_cast<glm::vec4>(value));
						break;
					}
					case PropertyType::Mat4:
					{
						UploadMat4(p.Name, std::any_cast<glm::mat4>(value));
						break;
					}
					case PropertyType::Texture:
					{
						auto texture = std::any_cast<Texture*>(value);
						if (texture)
						{
							texture->Bind(textureSlot);
							UploadInt(p.Name, textureSlot);
							textureSlot++;
						}
						break;
					}
					case PropertyType::Cubemap:
					{
						auto cubemap = std::any_cast<Cubemap*>(value);
						if (cubemap)
						{
							if (p.IsImage)
							{
								cubemap->BindImage(imageSlot++, 0, TextureAccess::Write);
								UploadInt(p.Name, imageSlot);
								imageSlot++;
							}
							else
							{
								cubemap->Bind(textureSlot);
								UploadInt(p.Name, textureSlot);
								textureSlot++;
							}
						}
						break;
					}
					case PropertyType::UBO:
					case PropertyType::SSBO:
					{
						auto buffer = std::any_cast<Buffer*>(value);
						if (buffer)
						{
							buffer->Bind(bufferBindingPoint++);
							UploadBuffer(p.Name, buffer);
						}
						break;
					}
					default:
					{
						PR_LOG_ERROR("Unknown PropertyType");
						break;
					}
				}
			}

		}
	}

	void Shader::SetRenderState()
	{
		RenderCommand::SetDepthCompareFunction(m_RenderStateBlock.depthState.compareFunction);
		if (m_RenderStateBlock.depthState.compareFunction != CompareFunction::Disabled)
			RenderCommand::SetDepthWriteEnabled(m_RenderStateBlock.depthState.writeEnabled);
		RenderCommand::SetCullMode(m_RenderStateBlock.cullMode);
		RenderCommand::SetBlendState(m_RenderStateBlock.blendState);
	}

	bool Shader::CheckBind()
	{
		if (this != s_PreShader)
		{
			Bind();
			s_PreShader = this;
			return false;
		}
		return true;
	}

	void Shader::SetInt(const std::string& name, int value)
	{
		s_PropertyValue[name] = { PropertyType::Int, value };
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		s_PropertyValue[name] = { PropertyType::Float, value };
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		s_PropertyValue[name] = { PropertyType::Float2 , value };
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		s_PropertyValue[name] = { PropertyType::Float3, value };
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		s_PropertyValue[name] = { PropertyType::Float4, value };
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		s_PropertyValue[name] = { PropertyType::Mat4, value };
	}

	void Shader::SetTexture(const std::string& name, Texture* value)
	{
		if(value)
			s_PropertyValue[name] = { PropertyType::Texture, value };
	}

	void Shader::SetCubemap(const std::string& name, Cubemap* value)
	{
		if (value)
			s_PropertyValue[name] = { PropertyType::Cubemap, value };
	}

	void Shader::SetBuffer(const std::string& name, Buffer* value)
	{
		if (!value)
			return;
		PropertyType bufferType;
		switch (value->GetBufferType())
		{
		case BufferType::UniformBuffer:
			bufferType = PropertyType::UBO;
			break;

		case BufferType::StorageBuffer:
			bufferType = PropertyType::SSBO;
			break;
		default:
			bufferType = PropertyType::UBO;
			break;
		}
		
		s_PropertyValue[name] = { bufferType, value };
	}

	bool Shader::HaveProperty(const std::string& property)
	{
		for (auto& p : m_PropertyData)
		{
			if (p.Name == property)
				return true;
		}
		return false;
	}

}