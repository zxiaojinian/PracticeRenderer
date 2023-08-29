#pragma once

#include "Core/Render/Asset/Shader.h"

#include <string>

namespace PR
{
	class Material
	{
	public:
		Material(const std::string& name);

		void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }
		const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }

		bool HaveProperty(const std::string& property);
		void UploadProperty() const;

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		void SetTexture(const std::string& name, Texture2D* value);
		//void SetIntArray(const std::string& name, int* values, uint32_t count);

	private :
		void SetProterty(const std::string& name, PropertyType type, std::any& value);

	private:
		std::shared_ptr<Shader> m_Shader;
		std::unordered_map<std::string, PropertyValue> m_PropertyValue;

		std::string m_Name;
	};
}

