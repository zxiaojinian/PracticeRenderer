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

		m_CameraMatrixUBO = Buffer::Create(1, sizeof(glm::mat4) * 3, BufferType::UniformBuffer, BufferUsage::Dynamic);
		Shader::SetBuffer("CameraMatrix", m_CameraMatrixUBO.get());
	}

	void GraphicsContext::SetViewProjectionMatrices(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
	{
		auto& vpMatrix = projectionMatrix * viewMatrix;
		glm::mat4 cameraMatrices[] = { viewMatrix , projectionMatrix, vpMatrix };
		m_CameraMatrixUBO->SetData(cameraMatrices, 0, 1, sizeof(glm::mat4) * 3);
	}

	void GraphicsContext::SetRenderTarget(RenderTexture* color, RenderTexture* depth, uint32_t colorLevel, CubemapFace colorCubeFace, uint32_t colorSlice, uint32_t depthLevel, CubemapFace depthCubeFace, uint32_t depthSlice)
	{
		PR_ASSERT(!color || !depth || (color && depth && (color->GetWidth() == depth->GetWidth()) && (color->GetHeight() == depth->GetHeight())), "color and depth is not same size");

		m_Framebuffer->AttachColorTexture(color, colorLevel, colorSlice);
		m_Framebuffer->AttachDepthTexture(depth, depthLevel, depthSlice);
		m_Framebuffer->Bind();

		uint32_t w = 0;
		uint32_t h = 0;
		if (color)
		{
			w = color->GetWidth();
			h = color->GetHeight();
		}
		else if (depth)
		{
			w = depth->GetWidth();
			h = depth->GetHeight();
		}
		RenderCommand::SetViewport(0, 0, w, h);
	}

	void GraphicsContext::SetRenderTarget(std::vector<RenderTexture>& colors, RenderTexture* depth)
	{
		if (colors.size() > 0 && depth)
		{
			bool result = (colors[0].GetWidth() == depth->GetWidth()) && (colors[0].GetHeight() == depth->GetHeight());
			for (size_t i = 0; i < colors.size() - 1; i++)
			{
				if (colors[i].GetWidth() != colors[i + 1].GetWidth() || colors[i].GetHeight() != colors[i + 1].GetHeight())
				{
					result = false;
					break;
				}
			}
			PR_ASSERT(result, "colors and depth is not same size");
		}

		m_Framebuffer->AttachColorTexture(colors);
		m_Framebuffer->AttachDepthTexture(depth);
		m_Framebuffer->Bind();

		uint32_t w = 0;
		uint32_t h = 0;
		if (colors.size() > 0)
		{
			w = colors[0].GetWidth();
			h = colors[0].GetHeight();
		}
		else if (depth)
		{
			w = depth->GetWidth();
			h = depth->GetHeight();
		}
		RenderCommand::SetViewport(0, 0, w, h);
		RenderCommand::SetViewport(0, 0, w, h);
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

	void GraphicsContext::DrawRenderer(const MeshRenderer& renderer, DrawingSettings& drawingSettings)
	{
		auto& mesh = renderer.GetMesh();
		auto& matrix = renderer.GetTransform().GetLocalToWorldMatrix();
		auto& inverseMatrix = renderer.GetTransform().GetWorldToLocalMatrix();
		if (drawingSettings.overrideMaterial)
		{
			Shader::SetMat4("Matrix_I_M", inverseMatrix);
			DrawMesh(*mesh, matrix, *drawingSettings.overrideMaterial);
		}
		else
		{
			auto& mats = renderer.GetMaterials();
			for (auto& mat : mats)
			{
				Shader::SetMat4("Matrix_I_M", inverseMatrix);
				DrawMesh(*mesh, matrix, *mat);
			}
		}
	}

	void GraphicsContext::DrawRenderers(const CullingResults& cullingResults, DrawingSettings& drawingSettings, const FilteringSettings& filteringSettings)
	{
		//TEMP
		auto& renderers = SceneManager::Get().GetCurrentScene()->GetMeshRenderers();
		for (auto renderer : renderers)
		{
			DrawRenderer(*renderer, drawingSettings);
		}
	}

	void GraphicsContext::DispatchCompute(ComputeShader& computeShader, uint32_t num_x, uint32_t num_y, uint32_t num_z)
	{
		computeShader.UploadProperty();
		RenderCommand::DispatchCompute(num_x, num_y, num_z);
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