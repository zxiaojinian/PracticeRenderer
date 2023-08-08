#pragma once
#include "Core/Scene/Component.h"

namespace PR
{
	class Transform : public Component
	{
	public:
		Transform(GameObject& go);
		~Transform();

	private:

	};

	Transform::Transform(GameObject& go)
		: Component(go)
	{
		m_DisallowMultipleComponent = true;
	}

	Transform::~Transform()
	{
	}
}
