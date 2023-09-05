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
		m_ShaderLibrary.Load("Assets/SimpleLit.glsl");
	}
}
