#include "pch.h"
#include "GraphicsContext.h"

#include "Core/Common.h"
#include "Core/Render/OpenGL/OpenGLContext.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/RenderCommand.h"

#include "Core/Scene/SceneManager.h"

namespace PR
{
	GraphicsContext::GraphicsContext()
	{
		m_Framebuffer = Framebuffer::Create();
	}

	void GraphicsContext::SetRenderTarget(RenderTexture& color, RenderTexture& depth)
	{
		m_Framebuffer->AttachColorTexture(color);
		m_Framebuffer->AttachDepthTexture(depth);
	}

	void GraphicsContext::SetRenderTarget(std::vector<RenderTexture&>& colors, RenderTexture& depth)
	{
		m_Framebuffer->AttachColorTexture(colors);
		m_Framebuffer->AttachDepthTexture(depth);
	}

	void GraphicsContext::ClearRenderTarget(bool clearDepth, bool clearColor, Color& backgroundColor)
	{
		RenderCommand::SetClearColor(backgroundColor.GetLinear());
		RenderCommand::Clear(clearDepth, clearColor);
	}

	void GraphicsContext::DrawRenderer(const MeshRenderer& renderer)
	{
		auto& mesh = renderer.GetMesh();
		mesh->Bind();
		auto& mats = renderer.GetMaterials();
		for (auto& mat : mats)
		{
			mat->UploadProperty();
		}
		RenderCommand::DrawIndexed(mesh->GetIndexCount());
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