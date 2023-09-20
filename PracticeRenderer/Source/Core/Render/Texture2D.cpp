#include "pch.h"
#include "Texture2D.h"

#include "Core/Common.h"
#include "Core/Render/RendererAPI.h"
#include "Core/Render/OpenGL/OpenGLTexture.h"
#include "Core/Render/RenderData/Color.h"

namespace PR
{
	std::shared_ptr<Texture2D> Texture2D::m_WhiteTexture = nullptr;
	std::shared_ptr<Texture2D> Texture2D::m_BlackTexture = nullptr;
	std::shared_ptr<Texture2D> Texture2D::m_GreyTexture = nullptr;

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& name, const Texture2DSpecification& specification)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PR_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(name, specification);
		}
		}

		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:
		{
			PR_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(path);
		}
		}
		PR_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	void Texture2D::Init()
	{
		Texture2DSpecification texture2DSpecification{};
		texture2DSpecification.Width = 1;
		texture2DSpecification.Height = 1;
		texture2DSpecification.Format = TextureFormat::RGBA;
		texture2DSpecification.GenerateMips = false;

		m_WhiteTexture = Create("Default WhiteTexture", texture2DSpecification);
		m_WhiteTexture->SetData(&Color::white, sizeof(Color::white));

		m_BlackTexture = Create("Default BlackTexture", texture2DSpecification);
		m_BlackTexture->SetData(&Color::black, sizeof(Color::black));

		m_GreyTexture = Create("Default GreyTexture", texture2DSpecification);
		m_GreyTexture->SetData(&Color::grey, sizeof(Color::grey));
	}
}
