#pragma once

#include "Core/Render/Asset/Shader.h"

#include <glm/glm.hpp>

typedef unsigned int GLenum;

namespace PR
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadInt(const std::string& name, int value) override;
		virtual void UploadFloat(const std::string& name, float value) override;
		virtual void UploadFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void UploadFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void UploadMat4(const std::string& name, const glm::mat4& value) override;
		virtual void UploadIntArray(const std::string& name, int* values, uint32_t count) override;

		virtual const std::string& GetName() const override { return m_Name; }

	protected:
		virtual void CollectProperty() override;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		uint32_t GetUniformLocation(const std::string& name);
	private:
		uint32_t m_RendererID;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocation;
	};
}

