#pragma once

#include "ShaderLibrary.h"
#include "Texture2DLibrary.h"
#include "ModelLoder.h"

namespace PR
{
	class Resources
	{
	public:
		Resources();
		static Resources& Get() { return *s_Instance; }

		void Init();

		//shader
		void AddShader(const std::string& name, const std::shared_ptr<Shader>& shader) { m_ShaderLibrary.Add(name, shader); }
		void AddShader(const std::shared_ptr<Shader>& shader) { m_ShaderLibrary.Add(shader); }
		std::shared_ptr<Shader> LoadShader(const std::string& filepath) { return m_ShaderLibrary.Load(filepath); }
		std::shared_ptr<Shader> LoadShader(const std::string& name, const std::string& filepath) { return m_ShaderLibrary.Load(name, filepath); }
		std::shared_ptr<Shader> GetShader(const std::string& name) { return m_ShaderLibrary.Get(name); }
		bool ExistsShader(const std::string& name) const { m_ShaderLibrary.Exists(name); }

		//texture
		void AddTexture(const std::string& name, const std::shared_ptr<Texture2D>& texture) { m_Texture2DLibrary.Add(name, texture); }
		void AddTexture(const std::shared_ptr<Texture2D>& texture) { m_Texture2DLibrary.Add(texture); }
		std::shared_ptr<Texture2D> LoadTexture(const std::string& filepath) { return m_Texture2DLibrary.Load(filepath); }
		std::shared_ptr<Texture2D> LoadTexture(const std::string& name, const std::string& filepath) { return m_Texture2DLibrary.Load(name, filepath); }
		std::shared_ptr<Texture2D> GetTexture(const std::string& name) { return m_Texture2DLibrary.Get(name); }
		bool ExistsTexture(const std::string& name) const { m_Texture2DLibrary.Exists(name); }

	private:
		static Resources* s_Instance;

		ShaderLibrary m_ShaderLibrary;
		Texture2DLibrary m_Texture2DLibrary;
		ModelLoder m_ModelLoder;
	};
}

