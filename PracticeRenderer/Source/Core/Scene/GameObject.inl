#pragma once

namespace PR
{
	template<typename T, typename ...Args>
	inline T& GameObject::AddComponent(Args && ...args)
	{

	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		return nullptr;
	}
}