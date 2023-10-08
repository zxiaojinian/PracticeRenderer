#include "pch.h"
#include "ComputeShader.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLComputeShader.h"
#include "Core/Render/Texture.h"
#include "Core/Render/Texture2D.h"

namespace PR
{
	bool ComputeShader::HaveProperty(const std::string& property)
	{
		for (auto& p : m_PropertyData)
		{
			if (p.Name == property)
				return true;
		}
		return false;
	}

	void ComputeShader::UploadProperty()
	{
		int textureSlot = 0;
		int bufferBindingPoint = 0;
		for (auto& p : m_PropertyData)
		{
			std::any value;
			if (m_PropertyValue.find(p.Name) != m_PropertyValue.end() && m_PropertyValue.at(p.Name).Type == p.Type)
			{
				value = m_PropertyValue.at(p.Name).Value;
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

	void ComputeShader::SetInt(const std::string& name, int value)
	{
		m_PropertyValue[name] = { PropertyType::Int, value };
	}

	void ComputeShader::SetFloat(const std::string& name, float value)
	{
		m_PropertyValue[name] = { PropertyType::Float, value };
	}

	void ComputeShader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		m_PropertyValue[name] = { PropertyType::Float2 , value };
	}

	void ComputeShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		m_PropertyValue[name] = { PropertyType::Float3, value };
	}

	void ComputeShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		m_PropertyValue[name] = { PropertyType::Float4, value };
	}

	void ComputeShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		m_PropertyValue[name] = { PropertyType::Mat4, value };
	}

	void ComputeShader::SetTexture(const std::string& name, Texture* value)
	{
		if (value)
			m_PropertyValue[name] = { PropertyType::Texture, value };
	}

	void ComputeShader::SetBuffer(const std::string& name, Buffer* value)
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

		m_PropertyValue[name] = { bufferType, value };
	}

	void ComputeShader::SetGlobalInt(const std::string& name, int value)
	{
		s_PropertyValue[name] = { PropertyType::Int, value };
	}

	void ComputeShader::SetGlobalFloat(const std::string& name, float value)
	{
		s_PropertyValue[name] = { PropertyType::Float, value };
	}

	void ComputeShader::SetGlobalFloat2(const std::string& name, const glm::vec2& value)
	{
		s_PropertyValue[name] = { PropertyType::Float2 , value };
	}

	void ComputeShader::SetGlobalFloat3(const std::string& name, const glm::vec3& value)
	{
		s_PropertyValue[name] = { PropertyType::Float3, value };
	}

	void ComputeShader::SetGlobalFloat4(const std::string& name, const glm::vec4& value)
	{
		s_PropertyValue[name] = { PropertyType::Float4, value };
	}

	void ComputeShader::SetGlobalMat4(const std::string& name, const glm::mat4& value)
	{
		s_PropertyValue[name] = { PropertyType::Mat4, value };
	}

	void ComputeShader::SetGlobalTexture(const std::string& name, Texture* value)
	{
		if (value)
			s_PropertyValue[name] = { PropertyType::Texture, value };
	}

	void ComputeShader::SetGlobalBuffer(const std::string& name, Buffer* value)
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


	std::shared_ptr<ComputeShader> ComputeShader::Create(const std::string& filepath)
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
				return std::make_shared<OpenGLComputeShader>(filepath);
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<ComputeShader> ComputeShader::Create(const std::string& name, const std::string& src)
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
				return std::make_shared<OpenGLComputeShader>(name, src);
			}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
