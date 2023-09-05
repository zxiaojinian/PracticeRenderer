#include "pch.h"
#include "Texture2DLibrary.h"

#include "Core/Common.h"

namespace PR
{
    void Texture2DLibrary::Add(const std::string& name, const std::shared_ptr<Texture2D>& texture2D)
    {
        PR_ASSERT(!Exists(name), "Texture already exists!");
        m_Texture2Ds[name] = texture2D;
    }

    void Texture2DLibrary::Add(const std::shared_ptr<Texture2D>& texture2D)
    {
        auto& name = texture2D->GetName();
        Add(name, texture2D);
    }

    std::shared_ptr<Texture2D> Texture2DLibrary::Load(const std::string& filepath)
    {
        auto texture = Texture2D::Create(filepath);
        Add(texture);
        return texture;
    }

    std::shared_ptr<Texture2D> Texture2DLibrary::Load(const std::string& name, const std::string& filepath)
    {
        auto texture = Texture2D::Create(filepath);
        Add(name, texture);
        return texture;
    }

    std::shared_ptr<Texture2D> Texture2DLibrary::Get(const std::string& name)
    {
        PR_ASSERT(Exists(name), "Texture not found!");
        return m_Texture2Ds[name];
    }

    bool Texture2DLibrary::Exists(const std::string& name) const
    {
        return m_Texture2Ds.find(name) != m_Texture2Ds.end();
    }
}
