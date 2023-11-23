#include "pch.h"
#include "OpenGLCubemap.h"

#include "Core/Common.h"
#include "OpenGLTextureUtil.h"

#include <glad/glad.h>

namespace PR
{
    OpenGLCubemap::OpenGLCubemap(const std::string& name, const CubemapSpecification& specification)
        : m_Name(name), m_CubemapSpecification(specification)
    {
        Init();
    }

    OpenGLCubemap::~OpenGLCubemap()
    {
        glDeleteTextures(1, &m_RendererID);
    }

    void OpenGLCubemap::SetFilterMode(TextureFilterMode filterMode)
    {
        m_CubemapSpecification.FilterMode = filterMode;
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(m_CubemapSpecification.FilterMode, m_CubemapSpecification.GenerateMips));
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(m_CubemapSpecification.FilterMode));
    }

    void OpenGLCubemap::SetWrapMode(TextureWrapMode wrapMode)
    {
        m_CubemapSpecification.WrapMode = wrapMode;
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(m_CubemapSpecification.WrapMode));
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(m_CubemapSpecification.WrapMode));
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GetOpenGLWrapMode(m_CubemapSpecification.WrapMode));
    }

    void OpenGLCubemap::SetTextureFormat(TextureFormat format)
    {
        if (m_CubemapSpecification.Format != format)
        {
            m_CubemapSpecification.Format = format;
            glDeleteTextures(1, &m_RendererID);
            Init();
        }
    }

    void OpenGLCubemap::SetGenerateMips(bool generateMips)
    {
        if (m_CubemapSpecification.GenerateMips != generateMips)
        {
            m_CubemapSpecification.GenerateMips = generateMips;
            if (m_CubemapSpecification.GenerateMips)
                glGenerateTextureMipmap(m_RendererID);
        }
    }

    void OpenGLCubemap::EnableCompare()
    {
        if (m_CubemapSpecification.Format == TextureFormat::D16_UNorm || m_CubemapSpecification.Format == TextureFormat::D24_UNorm ||
            m_CubemapSpecification.Format == TextureFormat::D24_UNorm_S8_UInt || m_CubemapSpecification.Format == TextureFormat::D32_SFloat
            || m_CubemapSpecification.Format == TextureFormat::D32_SFloat_S8_UInt)
        {
            glTextureParameteri(m_RendererID, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTextureParameteri(m_RendererID, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
    }

    void OpenGLCubemap::SetData(void* data, uint32_t x, uint32_t y, uint32_t width, uint32_t height, TexturePixelType dataType, CubemapFace face)
    {
        PR_ASSERT((width <= m_CubemapSpecification.Width && height <= m_CubemapSpecification.Width), "width/height error");

        if (width <= m_CubemapSpecification.Width && height <= m_CubemapSpecification.Width)
        {
            GLenum target = GetOpenGLCubemapFace(face);
            glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
            glTexSubImage2D(target, 0, x, y, width, height, GetOpenGLPixelFormat(m_CubemapSpecification.Format), GetOpenGLPixelType(dataType), data);
            if (m_CubemapSpecification.GenerateMips)
                glGenerateTextureMipmap(m_RendererID);
        }
    }

    void OpenGLCubemap::Bind(uint32_t slot) const
    {
        if(m_SmoothEdges)
            glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        else
            glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glBindTextureUnit(slot, m_RendererID);
    }

    void OpenGLCubemap::BindImage(uint32_t slot, uint32_t level, TextureAccess access, uint32_t slice) const
    {
        glBindImageTexture(slot, m_RendererID, level, true, 0, GetOpenGLTextureAccess(access), GetOpenGLInternalFormat(m_CubemapSpecification.Format));
    }

    void OpenGLCubemap::Init()
    {
        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        //glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        GLsizei mipmapCount = m_CubemapSpecification.GenerateMips ? static_cast<GLsizei>(std::floor(std::log2(m_CubemapSpecification.Width)) + 1) : 1;

        //glTexStorage2D(GL_TEXTURE_CUBE_MAP, mipmapCount, GetOpenGLInternalFormat(m_CubemapSpecification.Format), m_CubemapSpecification.Width, m_CubemapSpecification.Width);
        glTextureStorage2D(m_RendererID, mipmapCount, GetOpenGLInternalFormat(m_CubemapSpecification.Format), m_CubemapSpecification.Width, m_CubemapSpecification.Width);

        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GetOpenGLWrapMode(m_CubemapSpecification.WrapMode));
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GetOpenGLWrapMode(m_CubemapSpecification.WrapMode));
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GetOpenGLWrapMode(m_CubemapSpecification.WrapMode));

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GetOpenGLFilterMode(m_CubemapSpecification.FilterMode, m_CubemapSpecification.GenerateMips));
        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GetOpenGLFilterMode(m_CubemapSpecification.FilterMode));
    }
}
