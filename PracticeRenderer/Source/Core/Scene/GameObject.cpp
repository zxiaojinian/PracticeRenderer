#include "pch.h"
#include "GameObject.h"

#include "Core/Scene/SceneManager.h"

namespace PR
{
	GameObject::GameObject(const std::string& name, Scene* scene)
		: m_Name(name), m_Transform(AddComponent<Transform>())
	{
		if (scene)
		{
			m_BelongToScene = scene;
		}
		else
		{
			m_BelongToScene = SceneManager::Get().GetCurrentScene();
		}

		PR_ASSERT(m_BelongToScene, "The gameobject {0} does not belong to any scene", name);
		if (m_BelongToScene)
		{
			m_BelongToScene->AddGameObject(*this);
		}

		OnAwake();		
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

		if (m_BelongToScene)
		{
			for (auto& component : m_Components)
			{
				m_BelongToScene->OnComponentRemove(*component);
			}
		}

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
			OnActiveChange();
			m_Transform.SetParent(parent ? &(parent->m_Transform) : nullptr);
		}
	}

	void GameObject::RemoveComponent(Component& component)
	{
		for (auto c = m_Components.begin(); c != m_Components.end(); ++c)
		{
			if (c->get() == &component)
			{
				if(m_BelongToScene)
					m_BelongToScene->OnComponentRemove(component);
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
					break;
				}
			}
		}

		m_Parent = nullptr;
		m_Transform.SetParent(nullptr);
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

	void GameObject::OnEnable()
	{
		for (auto& c : m_Components)
		{
			c->OnEnable();
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
