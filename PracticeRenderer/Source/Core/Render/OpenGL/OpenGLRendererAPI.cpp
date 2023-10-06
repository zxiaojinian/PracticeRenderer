#include "pch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace PR
{
	void OpenGLRendererAPI::SetDepthCompareFunction(const CompareFunction& compareFunction)
	{
        switch (compareFunction) 
        {
            case CompareFunction::Disabled:
                glDisable(GL_DEPTH_TEST);
                break;
            case CompareFunction::Never:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_NEVER);
                break;
            case CompareFunction::Less:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                break;
            case CompareFunction::Equal:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_EQUAL);
                break;
            case CompareFunction::LessEqual:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LEQUAL);
                break;
            case CompareFunction::Greater:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_GREATER);
                break;
            case CompareFunction::NotEqual:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_NOTEQUAL);
                break;
            case CompareFunction::GreaterEqual:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_GEQUAL);
                break;
            case CompareFunction::Always:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_ALWAYS);
                break;
            default:
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                break;
        }
	}

    void OpenGLRendererAPI::SetDepthWriteEnabled(bool enbale)
    {
        if(enbale)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }

    void OpenGLRendererAPI::SetCullMode(CullMode cullMode)
    {
        switch (cullMode)
        {
            case PR::CullMode::Disabled:
                glDisable(GL_CULL_FACE);
                break;
            case PR::CullMode::Front:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);
                break;
            case PR::CullMode::Back:
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                break;
        }
    }

    void OpenGLRendererAPI::SetColorMask(ColorWriteMask colorMask)
    {
        GLboolean red = static_cast<uint8_t>(colorMask) & static_cast<uint8_t>(ColorWriteMask::Red);
        GLboolean green = static_cast<uint8_t>(colorMask) & static_cast<uint8_t>(ColorWriteMask::Green);
        GLboolean blue = static_cast<uint8_t>(colorMask) & static_cast<uint8_t>(ColorWriteMask::Blue);
        GLboolean alpha = static_cast<uint8_t>(colorMask) & static_cast<uint8_t>(ColorWriteMask::Alpha);

        glColorMask(red, green, blue, alpha);
    }

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const Color& color)
	{
		glClearColor(color.R, color.G, color.B, color.A);
	}

	void OpenGLRendererAPI::Clear(bool clearDepth, bool clearColor)
	{
		if (clearColor && clearDepth)
		{
            SetDepthWriteEnabled(true);
            SetColorMask(ColorWriteMask::All);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else if (clearColor)
		{
			glClear(GL_COLOR_BUFFER_BIT);
		}
		else if (clearDepth)
		{
            SetDepthWriteEnabled(true);
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	void OpenGLRendererAPI::DrawIndexed(uint32_t indexCount)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}
}