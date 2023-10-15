#pragma once

#include "Core/Render/Cubemap.h"

namespace PR
{
	class CubeMapLoader
	{
	public:
		CubeMapLoader();

		void Add(const std::shared_ptr<Cubemap>& cubemap);
		std::shared_ptr<Cubemap> Load(const std::string& name, const std::vector<std::string>& filepath, 
			TextureFilterMode filterMode = TextureFilterMode::Bilinear, TextureWrapMode wrapMode = TextureWrapMode::Clamp, bool generateMips = false);

		std::shared_ptr<Cubemap> Get(const std::string& name);

		bool Exists(const std::string& name) const;

	private:
		std::shared_ptr<Cubemap> LoadCubemap(const std::string& name, const std::vector<std::string>& filepath, 
			TextureFilterMode filterMode = TextureFilterMode::Bilinear, TextureWrapMode wrapMode = TextureWrapMode::Clamp, bool generateMips = false);

	private:
		std::unordered_map<std::string, std::shared_ptr<Cubemap>> m_Cubemaps;
	};
}

