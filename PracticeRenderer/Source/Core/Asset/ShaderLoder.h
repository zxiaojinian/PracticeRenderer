#pragma once

#include "Core/Render/Asset/Shader.h"
#include "Core/Render/Asset/ComputeShader.h"

namespace PR
{
	class ShaderLoder
	{
	public:
		void Add(const std::string& name, const std::shared_ptr<Shader>& shader);
		void Add(const std::shared_ptr<Shader>& shader);
		std::shared_ptr<Shader> Load(const std::string& filepath);
		std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);
		std::shared_ptr<Shader> Get(const std::string& name);
		bool Exists(const std::string& name) const;

		void AddComputeShader(const std::string& name, const std::shared_ptr<ComputeShader>& shader);
		void AddComputeShader(const std::shared_ptr<ComputeShader>& shader);
		std::shared_ptr<ComputeShader> LoadComputeShader(const std::string& filepath);
		std::shared_ptr<ComputeShader> LoadComputeShader(const std::string& name, const std::string& filepath);

		std::shared_ptr<ComputeShader> GetComputeShader(const std::string& name);
		bool ComputeShaderExists(const std::string& name) const;

	private:
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
		std::unordered_map<std::string, std::shared_ptr<Shader>> m_LoadCache;

		std::unordered_map<std::string, std::shared_ptr<ComputeShader>> m_ComputeShaders;
		std::unordered_map<std::string, std::shared_ptr<ComputeShader>> m_ComputeShaderLoadCache;
	};
}

