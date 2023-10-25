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


    namespace Util
    {
        GLenum GetOpenGLBlendMode(BlendMode blendmode)
        {
            switch (blendmode)
            {
                case BlendMode::Zero:
                    return GL_ZERO;
                case BlendMode::One:
                    return GL_ONE;
                case BlendMode::SrcColor:
                    return GL_SRC_COLOR;
                case BlendMode::DstColor:
                    return GL_DST_COLOR;
                case BlendMode::SrcAlpha:
                    return GL_SRC_ALPHA;
                case BlendMode::DstAlpha:
                    return GL_DST_ALPHA;
                case BlendMode::OneMinusSrcColor:
                    return GL_ONE_MINUS_SRC_COLOR;
                case BlendMode::OneMinusDstColor:
                    return GL_ONE_MINUS_DST_COLOR;
                case BlendMode::OneMinusSrcAlpha:
                    return GL_ONE_MINUS_SRC_ALPHA;
                case BlendMode::OneMinusDstAlpha:
                    return GL_ONE_MINUS_DST_ALPHA;
                default:
                    return GL_ONE;
            }
        }

        GLenum GetOpenGLBlendOp(BlendOp blendOP)
        {
            switch (blendOP)
            {
                case BlendOp::Add:
                    return GL_FUNC_ADD;
                case BlendOp::Subtract:
                    return GL_FUNC_SUBTRACT;
                case BlendOp::ReverseSubtract:
                    return GL_FUNC_REVERSE_SUBTRACT;
                case BlendOp::Min:
                    return GL_MIN;
                case BlendOp::Max:
                    return GL_MAX;
                default:
                    return GL_FUNC_ADD;
            }
        }
    }

    void OpenGLRendererAPI::SetBlendState(const BlendState& blendState)
    {
        SetColorMask(blendState.WriteMask);
        if (blendState.EnableBlend)
        {
            glEnable(GL_BLEND);
            glBlendFuncSeparate(Util::GetOpenGLBlendMode(blendState.SourceColorBlendMode), 
                Util::GetOpenGLBlendMode(blendState.DestinationColorBlendMode), 
                Util::GetOpenGLBlendMode(blendState.SourceAlphaBlendMode),
                Util::GetOpenGLBlendMode(blendState.DestinationAlphaBlendMode));

            glBlendEquationSeparate(
                Util::GetOpenGLBlendOp(blendState.ColorBlendOperation),
                Util::GetOpenGLBlendOp(blendState.AlphaBlendOperation)
            );
        }
        else
        {
            glDisable(GL_BLEND);
        }
    }

    void OpenGLRendererAPI::SetCullMode(const CullMode& cullMode)
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

    void OpenGLRendererAPI::SetColorMask(const ColorWriteMask& colorMask)
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

    void OpenGLRendererAPI::DispatchCompute(uint32_t num_x, uint32_t num_y, uint32_t num_z)
    {
        glDispatchCompute(num_x, num_y, num_z);
    }
}