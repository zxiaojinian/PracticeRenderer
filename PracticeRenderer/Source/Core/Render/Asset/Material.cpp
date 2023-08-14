#include "pch.h"
#include "Material.h"

namespace PR
{
	bool Material::HaveProperty(const std::string& property)
	{
		return false;
	}
	void Material::SetInt(const std::string& name, int value)
	{
	}
	void Material::SetFloat(const std::string& name, float value)
	{
	}
	void Material::SetFloat3(const std::string& name, const glm::vec3& value)
	{
	}
	void Material::SetFloat4(const std::string& name, const glm::vec4& value)
	{
	}
	void Material::SetMat4(const std::string& name, const glm::mat4& value)
	{
	}
	void Material::SetIntArray(const std::string& name, int* values, uint32_t count)
	{
	}
}