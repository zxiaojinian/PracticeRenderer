#include "pch.h"
#include "GameObject.h"

namespace PR
{
	GameObject::GameObject(int64_t id, const std::string& name)
		: m_ID(id), m_Name(name)
	{
	}

	GameObject::~GameObject()
	{
		if (IsActive())
			OnDisable();

		if (m_Awaked)
			OnDestroy();

		auto children = m_Children;
		for (auto child : children)
			child->DetachParent();
		DetachParent();

		for (auto child : m_Children)
		{
			delete child;
		}
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

	void GameObject::SetParent(GameObject* parent)
	{
		if (parent != m_Parent)
		{
			DetachParent();
			if (parent != nullptr)
			{
				m_Parent = parent;
				m_Parent->m_Children.push_back(this);
			}
		}
	}

	void GameObject::RemoveComponent(Component& component)
	{
		for (auto c = m_Components.begin(); c != m_Components.end(); ++c)
		{
			if (c->get() == &component)
			{
				m_Components.erase(c);
				return;
			}
		}
	}

	void GameObject::UpdateActive()
	{
		m_Active = IsActive();
		for (auto child : m_Children)
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

		for (auto child : m_Children)
			child->OnActiveChange();
	}

	void GameObject::DetachParent()
	{
		if (m_Parent)
		{
			for (auto c = m_Parent->m_Children.begin(); c != m_Parent->m_Children.end(); ++c)
			{
				if (*c == this)
				{
					m_Parent->m_Children.erase(c);
					return;
				}
			}
		}

		m_Parent = nullptr;
	}

	void GameObject::MarkAsDestroy()
	{
		m_Destroyed = true;
		for (auto child : m_Children)
		{
			child->MarkAsDestroy();
		}
	}

	void GameObject::OnAwake()
	{
		m_Awaked = true;
		for (auto& c : m_Components)
		{
			c->OnAwake();
		}
	}

	void GameObject::OnStart()
	{
		m_Started = true;
		for (auto& c : m_Components)
		{
			c->OnStart();
		}
	}

	void GameObject::OnEnable()
	{
		for (auto& c : m_Components)
		{
			c->OnEnable();
		}
	}

	void GameObject::OnFixedUpdate()
	{
		if (IsActive())
		{
			for (auto& c : m_Components)
			{
				c->OnFixedUpdate();
			}
		}
	}

	void GameObject::OnUpdate()
	{
		if (IsActive())
		{
			for (auto& c : m_Components)
			{
				c->OnUpdate();
			}
		}
	}

	void GameObject::OnLateUpdate()
	{
		if (IsActive())
		{
			for (auto& c : m_Components)
			{
				c->OnLateUpdate();
			}
		}
	}

	void GameObject::OnDisable()
	{
		for (auto& c : m_Components)
		{
			c->OnDisable();
		}
	}

	void GameObject::OnDestroy()
	{
		for (auto& c : m_Components)
		{
			c->OnDestroy();
		}
	}

}
