#include "pch.h"
#include "Component.h"

#include "Core/Scene/GameObject.h"

namespace PR
{
	Component::Component(GameObject& gameobject)
		: m_Gameobject(gameobject)
	{
	}

	Component::~Component()
	{
		if (m_Gameobject.IsActive())
		{
			OnDisable();
			OnDestroy();
		}
	}
}
