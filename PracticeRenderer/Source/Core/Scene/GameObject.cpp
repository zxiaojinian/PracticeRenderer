#include "pch.h"
#include "GameObject.h"

namespace PR
{
	GameObject::GameObject(int64_t id, const std::string& name)
	{

	}

	GameObject::~GameObject()
	{

	}

	void GameObject::SetActive(bool active)
	{
		if (active != m_ActiveSelf)
		{
			UpdateActive();
			m_ActiveSelf = active;
			OnActiveChange();
		}
	}

	void GameObject::SetParent(GameObject& p_parent)
	{
	}

	void GameObject::UpdateActive()
	{
		m_Active = IsActive();
		for (auto child : m_Childrens)
		{
			child->UpdateActive();
		}
	}

	void GameObject::OnActiveChange()
	{
		bool preActive = m_Active;
		m_Active = IsActive();

		if (!preActive && m_Active)
		{
			if (!m_Awaked)
			{
				OnAwake();
			}			
			OnEnable();
			if (!m_Started)
			{
				OnStart();
			}
		}

		if (preActive && !m_Active)
		{
			OnDisable();
		}

		for (auto child : m_Childrens)
			child->OnActiveChange();
	}

	void GameObject::OnAwake()
	{
	}

	void GameObject::OnStart()
	{
	}

	void GameObject::OnEnable()
	{
	}

	void GameObject::OnFixedUpdate()
	{
	}

	void GameObject::OnUpdate()
	{
	}

	void GameObject::OnLateUpdate()
	{
	}

	void GameObject::OnDisable()
	{
	}

	void GameObject::OnDestroy()
	{
	}

}
