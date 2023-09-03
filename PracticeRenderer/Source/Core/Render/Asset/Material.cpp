#include "pch.h"
#include "Material.h"

#include "Core/Common.h"
#include "Core/Render/RenderCommand.h"

namespace PR
{
	Material::Material(const std::string& name)
		:m_Name(name)
	{
	}

	bool Material::HaveProperty(const std::string& property)
	{
		return m_Shader->HaveProperty(property);
	}

	void Material::UploadProperty() const
	{
		m_Shader->UploadProperty(m_PropertyValue);
	}

	void Material::SetInt(const std::string& name, int value)
	{
		SetProterty(name, PropertyType::Property_Int, std::make_any<int>(value));
	}

	void Material::SetFloat(const std::string& name, float value)
	{
		SetProterty(name, PropertyType::Property_Float, std::make_any<float>(value));
	}

	void Material::SetFloat2(const std::string& name, const glm::vec2& value)
	{
		SetProterty(name, PropertyType::Property_Float2, std::make_any<glm::vec2>(value));
	}

	void Material::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		SetProterty(name, PropertyType::Property_Float3, std::make_any<glm::vec3>(value));
	}

	void Material::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		SetProterty(name, PropertyType::Property_Float4, std::make_any<glm::vec4>(value));
	}

	void Material::SetMat4(const std::string& name, const glm::mat4& value)
	{
		SetProterty(name, PropertyType::Property_Mat4, std::make_any<glm::mat4>(value));
	}

	void Material::SetTexture(const std::string& name, Texture* value)
	{
		SetProterty(name, PropertyType::Property_Texture, std::make_any<Texture*>(value));
	}

	void Material::SetProterty(const std::string& name, PropertyType type, std::any& value)
	{
		if (HaveProperty(name))
		{
			m_PropertyValue[name] = { type, value };
		}
		else
		{
			PR_LOG_WARN("Material " + m_Name + " doesn't have property " + name);
		}
	}
	//void Material::SetIntArray(const std::string& name, int* values, uint32_t count)
	//{
	//}
}