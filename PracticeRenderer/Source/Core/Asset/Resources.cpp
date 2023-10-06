#include "pch.h"
#include "Resources.h"

namespace PR
{
	Resources* Resources::s_Instance = nullptr;

	Resources::Resources()
	{
		s_Instance = this;
	}

	void Resources::Init()
	{
		m_ShaderLibrary.Load("Assets/Shader/PBR/PBRLit.glsl");
		Resources::Get().GetShader("PBRLit")->GetRenderStateBlock().cullMode = CullMode::Back;
		Resources::Get().GetShader("PBRLit")->GetRenderStateBlock().depthState.compareFunction = CompareFunction::LessEqual;
		Resources::Get().GetShader("PBRLit")->GetRenderStateBlock().depthState.writeEnabled = true;
	}
}
