#pragma once

#include "ShaderLoder.h"
#include "Texture2DLoder.h"
#include "CubeMapLoader.h"
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

		//cube map
		void AddCubemap(const std::shared_ptr<Cubemap>& cubemap) { m_CubeMapLoader.Add(cubemap); }

		std::shared_ptr<Cubemap> LoadCubemap(const std::string& name, const std::vector<std::string>& filepath, 
			TextureFilterMode filterMode = TextureFilterMode::Bilinear, TextureWrapMode wrapMode = TextureWrapMode::Clamp, bool generateMips = false) 
		{ return m_CubeMapLoader.Load(name, filepath, filterMode, wrapMode, generateMips); }

		std::shared_ptr<Cubemap> GetCubemap(const std::string& name) { return m_CubeMapLoader.Get(name); }
		bool ExistsCubemap(const std::string& name) const { m_CubeMapLoader.Exists(name); }

		//model
		GameObject* LoadModel(const std::string& path, Scene* scene) { return m_ModelLoder.LoadModel(path, scene); }
		std::vector<std::shared_ptr<Mesh>> LoadMeshes(const std::string& path) { return m_ModelLoder.LoadMeshes(path); }

	private:
		static Resources* s_Instance;

		ShaderLoder m_ShaderLibrary;
		Texture2DLoder m_Texture2DLibrary;
		CubeMapLoader m_CubeMapLoader;
		ModelLoder m_ModelLoder;		
	};
}

