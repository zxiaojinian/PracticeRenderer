#include "pch.h"
#include "GraphicsContext.h"

#include "Core/Common.h"
#include "Core/Render/OpenGL/OpenGLContext.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/RenderCommand.h"

#include "Core/Scene/SceneManager.h"
#include "Core/Scene/Components/Transform.h"
#include "Core/Event/EventData/ApplicationEvent.h"

namespace PR
{
	void GraphicsContext::Init()
	{
		m_Framebuffer = Framebuffer::Create();
	}

	void GraphicsContext::SetRenderTarget(RenderTexture& color, RenderTexture& depth)
	{
		PR_ASSERT((color.GetWidth() == depth.GetWidth()) && (color.GetHeight() == depth.GetHeight()), "color and depth is not same size");

		m_Framebuffer->AttachColorTexture(color);
		m_Framebuffer->AttachDepthTexture(depth);
		m_Framebuffer->Bind();
		RenderCommand::SetViewport(0, 0, color.GetWidth(), color.GetHeight());
	}

	void GraphicsContext::SetRenderTarget(std::vector<RenderTexture>& colors, RenderTexture& depth)
	{
		PR_ASSERT(colors.size(), " no color RT");
		bool result = (colors[0].GetWidth() == depth.GetWidth()) && (colors[0].GetHeight() == depth.GetHeight());
		for (size_t i = 0; i < colors.size() - 1 ; i++)
		{
			if (colors[i].GetWidth() != colors[i + 1].GetWidth() || colors[i].GetHeight() != colors[i + 1].GetHeight())
			{
				result = false;
				break;
			}
		}
		PR_ASSERT(result, "colors and depth is not same size");

		m_Framebuffer->AttachColorTexture(colors);
		m_Framebuffer->AttachDepthTexture(depth);
		m_Framebuffer->Bind();
		RenderCommand::SetViewport(0, 0, colors[0].GetWidth(), colors[0].GetHeight());
	}

	void GraphicsContext::SetBackBuffer()
	{
		m_Framebuffer->Unbind();
	}

	void GraphicsContext::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		PR_ASSERT(x < width && y < height && x >= 0 && y >= 0 && width >= 1 && height > 1, "SetViewport() parameter is error");
		RenderCommand::SetViewport(x, y, width, height);
	}

	void GraphicsContext::SetViewport(glm::u32vec4 viewPort)
	{
		SetViewport(viewPort.x, viewPort.y, viewPort.z, viewPort.w);
	}

	void GraphicsContext::ClearRenderTarget(bool clearDepth, bool clearColor, Color& backgroundColor)
	{
		RenderCommand::SetClearColor(backgroundColor.GetLinear());
		RenderCommand::Clear(clearDepth, clearColor);
	}

	void GraphicsContext::DrawMesh(Mesh& mesh, const glm::mat4& matrix, Material& material)
	{
		mesh.Bind();
		Shader::SetMat4("Matrix_M", matrix);
		material.Bind();
		RenderCommand::DrawIndexed(mesh.GetIndexCount());
	}

	void GraphicsContext::DrawRenderer(const MeshRenderer& renderer)
	{
		auto& mesh = renderer.GetMesh();
		auto& matrix = renderer.GetTransform().GetLocalToWorldMatrix();
		auto& inverseMatrix = renderer.GetTransform().GetWorldToLocalMatrix();
		auto& mats = renderer.GetMaterials();
		for (auto& mat : mats)
		{
			Shader::SetMat4("Matrix_I_M", inverseMatrix);
			DrawMesh(*mesh, matrix, *mat);
		}
	}

	void GraphicsContext::DrawRenderers(const CullingResults& cullingResults, const DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings)
	{
		//TEMP
		auto& renderers = SceneManager::Get().GetCurrentScene()->GetMeshRenderers();
		for (auto renderer : renderers)
		{
			DrawRenderer(*renderer);
		}
	}

	std::unique_ptr<GraphicsContext> GraphicsContext::Create()
	{
		switch (RendererAPI::GetAPI())
		{
			case RendererAPI::API::None:    
			{
				PR_ASSERT(false, "BackendsAPI::None is currently not supported!");
				return nullptr;
			}
			case RendererAPI::API::OpenGL:
			{
				return std::make_unique<OpenGLContext>();
			}
		}

		PR_ASSERT(false, "Unknown BackendsAPI!");
		return nullptr;
	}

}