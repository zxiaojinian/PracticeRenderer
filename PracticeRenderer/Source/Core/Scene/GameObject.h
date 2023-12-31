#pragma once

#include "Core/Scene/Component.h"
#include "Core/Scene/Components/Transform.h"
#include "Core/Scene/Scene.h"

namespace PR
{
	class GameObject
	{
	public:
		GameObject(const std::string& name, Scene* scene);
		~GameObject();

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		void SetActive(bool active);
		bool IsSelfActive() const { return m_ActiveSelf; }
		bool IsActive() const{ return m_ActiveSelf && (m_Parent ? m_Parent->IsActive() : true); }

		void SetParent(GameObject* parent);
		GameObject* GetParent() const { return m_Parent; }
		std::vector<GameObject*>& GetChildren() { return m_Children; }

		template<typename T>
		T& AddComponent();

		template<typename T>
		T* GetComponent();

		void RemoveComponent(Component& component);

		Transform& GetTransform () const { return m_Transform; }
	private:

		void UpdateActive();
		void OnActiveChange();
		void DetachParent();
		void MarkAsDestroy();

	public:
		void OnAwake();
		void OnEnable();
		void OnStart();
		void OnFixedUpdate();
		void OnUpdate();
		void OnLateUpdate();
		void OnDisable();
		void OnDestroy();

	private:
		Scene* m_BelongToScene = nullptr;

		std::string m_Name;
		bool m_ActiveSelf = true;

		GameObject* m_Parent = nullptr;
		std::vector<GameObject*> m_Children;

		std::vector<std::shared_ptr<Component>> m_Components;

		Transform& m_Transform;

		//是否已经执行过OnAwake
		bool m_Awaked = false;
		bool m_Started = false;
		bool m_Destroyed = false;

		bool m_Active = false;
	};
}

#include "GameObject.inl"