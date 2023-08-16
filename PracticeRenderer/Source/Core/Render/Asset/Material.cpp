#include "pch.h"
#include "Material.h"

#include "Core/Common.h"

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

	void Material::SetInt(const std::string& name, int value)
	{
		
	}

	void Material::SetFloat(const std::string& name, float value)
	{
		m_PropertyValue[name] = { PropertyType::Property_Float, value };
	}

	void Material::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		m_PropertyValue[name] = { PropertyType::Property_Float3, value };
	}

	void Material::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		m_PropertyValue[name] = { PropertyType::Property_Float4, value };
	}

	void Material::SetMat4(const std::string& name, const glm::mat4& value)
	{
		m_PropertyValue[name] = { PropertyType::Property_Mat4, value };
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