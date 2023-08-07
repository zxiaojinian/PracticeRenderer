#include "pch.h"
#include "Component.h"

#include "Core/Scene/GameObject.h"

namespace PR
{
	Component::Component(GameObject& gameobject)
		: gameobject(gameobject)
	{
	}

	Component::~Component()
	{
		if (gameobject.IsActive())
		{
			OnDisable();
			OnDestroy();
		}
	}
}
