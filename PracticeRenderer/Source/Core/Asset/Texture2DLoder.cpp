#include "pch.h"
#include "Texture2DLoder.h"

#include "Core/Common.h"

#include <stb_image.h>

namespace PR
{
    Texture2DLoder::Texture2DLoder()
    {
        Texture2D::Init();
    }

    void Texture2DLoder::Add(const std::string& name, const std::shared_ptr<Texture2D>& texture2D)
    {
        PR_ASSERT(!Exists(name), "Texture already exists!");
        m_Texture2Ds[name] = texture2D;
    }

    void Texture2DLoder::Add(const std::shared_ptr<Texture2D>& texture2D)
    {
        auto& name = texture2D->GetName();
        Add(name, texture2D);
    }

    std::shared_ptr<Texture2D> Texture2DLoder::Load(const std::string& filepath)
    {
        if (m_LoadCache.find(filepath) != m_LoadCache.end())
            return m_LoadCache[filepath];

        auto texture = LoadTexture(filepath);
        if (texture != nullptr)
        {
            Add(texture);
        }
        return texture;
    }

    std::shared_ptr<Texture2D> Texture2DLoder::Load(const std::string& name, const std::string& filepath)
    {
        if (m_LoadCache.find(filepath) != m_LoadCache.end())
            return m_LoadCache[filepath];

        auto texture = LoadTexture(filepath);
        if (texture != nullptr)
        {
            Add(name, texture);
        }
        return texture;
    }

    std::shared_ptr<Texture2D> Texture2DLoder::Get(const std::string& name)
    {
        PR_ASSERT(Exists(name), "Texture not found!");
        return m_Texture2Ds[name];
    }

    bool Texture2DLoder::Exists(const std::string& name) const
    {
        return m_Texture2Ds.find(name) != m_Texture2Ds.end();
    }

    std::shared_ptr<Texture2D> Texture2DLoder::LoadTexture(const std::string& filepath)
    {
        auto lastSlash = filepath.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = filepath.rfind('.');
        auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
        std::string name = filepath.substr(lastSlash, count);

        stbi_set_flip_vertically_on_load(1);
        int width, height, channels;
        stbi_uc* data = nullptr;
        data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (data)
        {
            Texture2DSpecification texture2DSpecification{};
            texture2DSpecification.Width = width;
            texture2DSpecification.Height = height;

            if (channels == 1)
                texture2DSpecification.Format = TextureFormat::R8_UNorm;
            else if (channels == 3)
            {
                if (name.find("Diffuse") != std::string::npos)
                    texture2DSpecification.Format = TextureFormat::R8G8B8_SRGB;
                else
                    texture2DSpecification.Format = TextureFormat::R8G8B8_UNorm;
            }
            else if (channels == 4)
            {
                if (name.find("Diffuse") != std::string::npos)
                    texture2DSpecification.Format = TextureFormat::R8G8B8A8_SRGB;
                else
                    texture2DSpecification.Format = TextureFormat::R8G8B8A8_UNorm;
            }

            auto texture = Texture2D::Create(name, texture2DSpecification);
            texture->SetData(data, 0, 0, width, height, TexturePixelType::UNSIGNED_BYTE);
            stbi_image_free(data);

            PR_LOG_INFO("Load Texture:{0}", filepath);
            m_LoadCache[filepath] = texture;
            return texture;
        }
        else
        {
            PR_LOG_ERROR("Failed to load texture {0}", filepath);
            return nullptr;
        }
    }
}
