#include "pch.h"
#include "PostProcessPass.h"

#include "Core/Asset/Resources.h"
#include "Core/Input/Input.h"
namespace PR
{
	PostProcessPass::PostProcessPass(RenderPassEvent renderPassEvent) : RenderPass(renderPassEvent)
	{
		//bloom
		std::shared_ptr<Shader> bloomDownsSamplingShader = Resources::Get().LoadShader("Assets/Shader/PostProcess/BloomDownsSampling.shader");
		std::shared_ptr<Shader> bloomUpSamplingShader = Resources::Get().LoadShader("Assets/Shader/PostProcess/BloomUpSampling.shader");
		//Temp
		bloomDownsSamplingShader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::Disabled;
		bloomDownsSamplingShader->GetRenderStateBlock().depthState.writeEnabled = false;
		bloomDownsSamplingShader->GetRenderStateBlock().cullMode = CullMode::Disabled;

		bloomUpSamplingShader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::Disabled;
		bloomUpSamplingShader->GetRenderStateBlock().depthState.writeEnabled = false;
		bloomUpSamplingShader->GetRenderStateBlock().cullMode = CullMode::Disabled;

		bloomUpSamplingShader->GetRenderStateBlock().blendState.EnableBlend = true;
		bloomUpSamplingShader->GetRenderStateBlock().blendState.SourceColorBlendMode = BlendMode::One;
		bloomUpSamplingShader->GetRenderStateBlock().blendState.DestinationColorBlendMode = BlendMode::One;
		bloomUpSamplingShader->GetRenderStateBlock().blendState.ColorBlendOperation = BlendOp::Add;

		m_BloomDownsSamplingMat = std::make_shared<Material>("BloomDownsSamplingMat");
		m_BloomUpSamplingMat = std::make_shared<Material>("BloomUpSamplingMat");
		m_BloomDownsSamplingMat->SetShader(bloomDownsSamplingShader);
		m_BloomUpSamplingMat->SetShader(bloomUpSamplingShader);

		//uber
		std::shared_ptr<Shader> uberShader = Resources::Get().LoadShader("Assets/Shader/PostProcess/Uber.shader");
		//Temp
		uberShader->GetRenderStateBlock().depthState.compareFunction = CompareFunction::Disabled;
		uberShader->GetRenderStateBlock().depthState.writeEnabled = false;
		uberShader->GetRenderStateBlock().cullMode = CullMode::Disabled;
		m_UberMat = std::make_shared<Material>("UberMat");
		m_UberMat->SetShader(uberShader);
	}

	void PostProcessPass::Execute(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		DoBloom(graphicsContext, renderingData);

		//uber
		graphicsContext.SetBackBuffer();
		graphicsContext.SetViewport(renderingData.cameraData.pixelRect);
		Shader::SetTexture("srcTexture", m_Source.get());
		graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_UberMat);
	}

	void PostProcessPass::Setup(std::shared_ptr<RenderTexture>& source)
	{
		m_Source = source;
	}

	void PostProcessPass::DoBloom(GraphicsContext& graphicsContext, const RenderingData& renderingData)
	{
		uint32_t w = renderingData.cameraData.pixelRect.z;
		uint32_t h = renderingData.cameraData.pixelRect.w;

		if (w != m_InitWidth || h != m_InitHeight)
		{
			m_InitWidth = w;
			m_InitHeight = h;

			uint32_t maxSize = (std::max)(w, h);
			uint32_t mipMapCount = static_cast<uint32_t>(std::floor(std::log2(maxSize)));
			const uint32_t minMipCount = 0;
			mipMapCount = std::clamp(mipMapCount, (uint32_t)0, k_MaxMipCount);

			m_BloomMipTexture.clear();
			RenderTextureSpecification colorSpecification = { w, h, 1, TextureFormat::R11G11B10_UFloatPack32, TextureWrapMode::Clamp, TextureFilterMode::Bilinear, false };
			for (uint32_t mipIndex = 0; mipIndex < mipMapCount; ++mipIndex)
			{
				w = (std::max)(w / 2, (uint32_t)1);
				h = (std::max)(h / 2, (uint32_t)1);
				colorSpecification.Width = w;
				colorSpecification.Height = h;
				auto mipTexture = RenderTexture::Create("mipTex" + std::to_string(mipIndex), colorSpecification);
				m_BloomMipTexture.push_back(mipTexture);
			}
		}

		//Bloom使用模糊来使亮部溢出，越模糊溢出越多，但越模糊就会使亮部越暗，既要更广的溢出，又要保证亮度，需要低模糊程度到高模糊程度的模糊结果，最后相加。
		//而高模糊程度意味着更大的滤波核，更多采样。对分辨率2R的图，采用2N样本模糊，近似于，对分辨率R的图，采用N样本模糊，再复原到2R分辨率。

		//downsample
		m_BloomDownsSamplingMat->SetTexture("srcTexture", m_Source.get());
		m_BloomDownsSamplingMat->SetFloat2("srcTexelSize", glm::vec2(1.0f / m_Source->GetWidth(), 1.0f / m_Source->GetHeight()));
		for (size_t i = 0; i < m_BloomMipTexture.size(); i++)
		{
			graphicsContext.SetRenderTarget(m_BloomMipTexture[i].get(), nullptr);
			graphicsContext.ClearRenderTarget(false, true, Color::clear);
			graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_BloomDownsSamplingMat);

			m_BloomDownsSamplingMat->SetTexture("srcTexture", m_BloomMipTexture[i].get());
			m_BloomDownsSamplingMat->SetFloat2("srcTexelSize", glm::vec2(1.0f / m_BloomMipTexture[i]->GetWidth(), 1.0f / m_BloomMipTexture[i]->GetHeight()));
		}

		//upsample
		m_BloomUpSamplingMat->SetFloat("filterRadius", 0.005f);
		for (size_t i = m_BloomMipTexture.size() - 1; i > 0; i--)
		{
			graphicsContext.SetRenderTarget(m_BloomMipTexture[i-1].get(), nullptr);
			m_BloomUpSamplingMat->SetTexture("srcTexture", m_BloomMipTexture[i].get());

			graphicsContext.DrawMesh(Mesh::FullScreenMesh, glm::mat4(1.0f), *m_BloomUpSamplingMat);
		}

		//Temp
		if (Input::IsKeyPressed(PR_KEY_KP_ADD))
		{
			bloomStrength += 0.001f;
		}
		else if(Input::IsKeyPressed(PR_KEY_KP_SUBTRACT))
		{
			bloomStrength -= 0.001f;
			bloomStrength = (std::max)(bloomStrength, 0.0f);
		}
		Shader::SetTexture("bloomTexture", m_BloomMipTexture[0].get());
		Shader::SetFloat("bloomStrength", bloomStrength);
	}
}
