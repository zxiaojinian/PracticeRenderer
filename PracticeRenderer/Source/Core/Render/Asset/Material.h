#pragma once

#include "Core/Render/Asset/Shader.h"

namespace PR
{
	class Material
	{
	public:
		void SetShader(const std::shared_ptr<Shader>& shader) { m_Shader = shader; }
		const std::shared_ptr<Shader>& GetShader() const { return m_Shader; }

		bool HaveProperty(const std::string& property);

		void SetInt(const std::string& name, int value);
		void SetFloat(const std::string& name, float value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);
		void SetMat4(const std::string& name, const glm::mat4& value);
		void SetIntArray(const std::string& name, int* values, uint32_t count);

	private:
		std::shared_ptr<Shader> m_Shader;
	};
}

