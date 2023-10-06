#include "pch.h"
#include "ShaderLoder.h"

#include "Core/Common.h"

namespace PR
{
	void ShaderLoder::Add(const std::string& name, const std::shared_ptr<Shader>& shader)
	{
		PR_ASSERT(!Exists(name), "Shader already exists!");
		m_Shaders[name] = shader;
	}

	void ShaderLoder::Add(const std::shared_ptr<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}

	std::shared_ptr<Shader> ShaderLoder::Load(const std::string& filepath)
	{
		if (m_LoadCache.find(filepath) != m_LoadCache.end())
			return m_LoadCache[filepath];
		auto shader = Shader::Create(filepath);
		Add(shader);
		m_LoadCache[filepath] = shader;
		return shader;
	}

	std::shared_ptr<Shader> ShaderLoder::Load(const std::string& name, const std::string& filepath)
	{
		if (m_LoadCache.find(filepath) != m_LoadCache.end())
			return m_LoadCache[filepath];
		auto shader = Shader::Create(filepath);
		Add(name, shader);
		m_LoadCache[filepath] = shader;
		return shader;
	}

	std::shared_ptr<Shader> ShaderLoder::Get(const std::string& name)
	{
		PR_ASSERT(Exists(name), "Shader not found!");
		return m_Shaders[name];
	}

	bool ShaderLoder::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}
