#pragma once

#include "Core/Render/Asset/ComputeShader.h"

#include <glm/glm.hpp>

namespace PR
{
	class OpenGLComputeShader : public ComputeShader
	{
	public:
		OpenGLComputeShader(const std::string& filePath);
		OpenGLComputeShader(const std::string& name, const std::string& src);
		virtual ~OpenGLComputeShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void UploadInt(const std::string& name, int value) override;
		virtual void UploadFloat(const std::string& name, float value) override;
		virtual void UploadFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void UploadFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void UploadFloat4(const std::string& name, const glm::vec4& value) override;
		virtual void UploadMat4(const std::string& name, const glm::mat4& value) override;
		virtual void UploadBuffer(const std::string& name, Buffer* value) override;

		virtual const std::string& GetName() const override { return m_Name; }

	protected:
		virtual void CollectProperty() override;

	private:
		std::string ProcessShader(const std::string& source, const std::string& filePath);
		std::string GetIncludeFilePath(const std::string& includePath, const std::string& filePath);
		void Compile(const std::string shaderSource);
		uint32_t GetUniformLocation(const std::string& name);
		uint32_t GetBufferIndex(const std::string& name, BufferType bufferType);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
		std::unordered_map<std::string, int> m_UniformLocation;
		std::unordered_map<std::string, int> m_BufferIndex;
	};
}


