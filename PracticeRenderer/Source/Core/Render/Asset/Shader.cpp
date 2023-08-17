#include "pch.h"
#include "Shader.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLShader.h"

namespace PR
{
	std::unordered_map<std::string, PropertyValue> Shader::s_PropertyValue;

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

	void ShaderLibrary::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		PR_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filepath)
	{
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		return shader;
	}

	std::shared_ptr<Shader> ShaderLibrary::Get(const std::string& name)
	{
		PR_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}

	void Shader::UploadProperty(std::unordered_map<std::string, PropertyValue>& materialValue)
	{
		Bind();
		for (auto& p : m_PropertyData)
		{
			std::any value;
			if (materialValue.find(p.Name) != materialValue.end() && materialValue[p.Name].Type == p.Type)
			{
				value = materialValue[p.Name].Value;
			}
			else if (s_PropertyValue.find(p.Name) != s_PropertyValue.end() && s_PropertyValue[p.Name].Type == p.Type)
			{
				value = s_PropertyValue[p.Name].Value;
			}

			if (value.has_value())
			{
				switch (p.Type)
				{
					case PropertyType::Property_Int:
					{
						UploadInt(p.Name, std::any_cast<int>(value));
						break;							
					}
					case PropertyType::Property_Float:
					{
						UploadFloat(p.Name, std::any_cast<float>(value));
						break;
					}
					case PropertyType::Property_Float2:
					{
						UploadFloat2(p.Name, std::any_cast<glm::vec2>(value));
						break;
					}
					case PropertyType::Property_Float3:
					{
						UploadFloat3(p.Name, std::any_cast<glm::vec3>(value));
						break;
					}
					case PropertyType::Property_Float4:
					{
						UploadFloat4(p.Name, std::any_cast<glm::vec4>(value));
						break;
					}
					case PropertyType::Property_Mat4:
					{
						UploadMat4(p.Name, std::any_cast<glm::mat4>(value));
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

	void Shader::SetInt(const std::string& name, int value)
	{
		s_PropertyValue[name] = { PropertyType::Property_Int, value };
	}

	void Shader::SetFloat(const std::string& name, float value)
	{
		s_PropertyValue[name] = { PropertyType::Property_Float, value };
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		s_PropertyValue[name] = { PropertyType::Property_Float2 , value };
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		s_PropertyValue[name] = { PropertyType::Property_Float3, value };
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		s_PropertyValue[name] = { PropertyType::Property_Float4, value };
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		s_PropertyValue[name] = { PropertyType::Property_Mat4, value };
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