#include "pch.h"
#include "CubeMapLoader.h"

#include "Core/Common.h"

#include <stb_image.h>

namespace PR
{
	CubeMapLoader::CubeMapLoader()
	{
	}

	void CubeMapLoader::Add(const std::shared_ptr<Cubemap>& cubemap)
	{
		auto& name = cubemap->GetName();
		PR_ASSERT(!Exists(name), "Cubemap already exists!");
		m_Cubemaps[name] = cubemap;
	}

	std::shared_ptr<Cubemap> CubeMapLoader::Load(const std::string& name, const std::vector<std::string>& filepath, TextureFilterMode filterMode, TextureWrapMode wrapMode, bool generateMips)
	{
		auto cubemap = LoadCubemap(name, filepath, filterMode, wrapMode, generateMips);
		if (cubemap != nullptr)
		{
			Add(cubemap);
		}
		return cubemap;
	}

	std::shared_ptr<Cubemap> CubeMapLoader::Get(const std::string& name)
	{
		PR_ASSERT(Exists(name), "Cubemap not found!");
		return m_Cubemaps[name];
	}

	bool CubeMapLoader::Exists(const std::string& name) const
	{
		return m_Cubemaps.find(name) != m_Cubemaps.end();
	}

	std::shared_ptr<Cubemap> CubeMapLoader::LoadCubemap(const std::string& name, const std::vector<std::string>& filepath, TextureFilterMode filterMode, TextureWrapMode wrapMode, bool generateMips)
	{
		if (filepath.size() < 0)
		{
			PR_LOG_ERROR("CubeMapLoader:filepath size must be 6");
			return nullptr;
		}
		auto lastSlash = filepath[0].find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath[0].rfind('.');
		//auto count = lastDot == std::string::npos ? filepath[0].size() - lastSlash : lastDot - lastSlash;
		//std::string name = filepath[0].substr(lastSlash, count);

		std::string extension = lastDot == std::string::npos ? "" : filepath[0].substr(lastDot + 1);

		std::shared_ptr<Cubemap> cubeMap = nullptr;

		//opengl cubemap从上往下，和普通纹理从下往上不同，
		stbi_set_flip_vertically_on_load(0);
		for (size_t i = 0; i < filepath.size(); i++)
		{
			int width, height, channels;
			void* data = nullptr;
			if(extension == "hdr")
				data = stbi_loadf(filepath[i].c_str(), &width, &height, &channels, 0);
			else
				data = stbi_load(filepath[i].c_str(), &width, &height, &channels, 0);

			if (data)
			{
				if (cubeMap == nullptr)
				{
					CubemapSpecification specification{};
					specification.FilterMode = filterMode;
					specification.GenerateMips = generateMips;
					specification.WrapMode = wrapMode;
					specification.Width = width;

					if (channels == 1)
						specification.Format = extension == "hdr" ? TextureFormat::D16_UNorm : TextureFormat::R8_UNorm;
					else if (channels == 3)
					{
						specification.Format = extension == "hdr" ? TextureFormat::R16G16B16_SFloat : TextureFormat::R8G8B8_SRGB;
					}
					else if (channels == 4)
					{
						specification.Format = extension == "hdr" ? TextureFormat::R16G16B16A16_SFloat : TextureFormat::R8G8B8A8_SRGB;
					}
					cubeMap = Cubemap::Create(name, specification);
				}
				cubeMap->SetData(data, 0, 0, width, height, extension == "hdr" ? TexturePixelType::FLOAT : TexturePixelType::UNSIGNED_BYTE, static_cast<CubemapFace>(i));
				stbi_image_free(data);

				auto path = filepath[i];
				PR_LOG_INFO("Load CubeMap{0}: load texture {1}", name, path);

			}
			else
			{
				auto path = filepath[i];
				PR_LOG_ERROR("Load CubeMap{0}: failed to load texture {1}", name, path);
			}
		}
		return cubeMap;
	}
}