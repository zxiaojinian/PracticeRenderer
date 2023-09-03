#pragma once

#include "Core/Common.h"

#include <type_traits>
#include <memory>

namespace PR
{
	template<typename T>
	inline T& GameObject::AddComponent()
	{
		T* component = GetComponent<T>();
		if (component && reinterpret_cast<Component*>(component)->GetDisallowMultipleComponent())
		{
			return *component;
		}
		else
		{
			m_Components.insert(m_Components.begin(), std::make_shared<T>(*this));
			if(m_BelongToScene)
				m_BelongToScene->OnComponentAdd(*(m_Components.front()));
			auto addComponent = m_Components.front().get();
			if (IsActive())
			{
				addComponent->OnAwake();
				addComponent->OnEnable();
				addComponent->OnStart();
			}
			return *dynamic_cast<T*>(addComponent);
		}
	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		std::shared_ptr<T> c(nullptr);
		for (auto i = m_Components.begin(); i != m_Components.end(); ++i)
		{
			c = std::dynamic_pointer_cast<T>(*i);
			if (c) return c.get();
		}
		return nullptr;
	}
}