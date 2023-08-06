#pragma once

#include "Core/Scene/Component.h"

namespace PR
{
	class GameObject
	{

	public:
		GameObject(int64_t id, const std::string& name);
		virtual ~GameObject();

		const std::string& GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		void SetID(int64_t id) { m_ID = id; }
		int64_t GetID() const { return m_ID; }

		void SetActive(bool active);
		bool IsSelfActive() const { return m_ActiveSelf; }
		bool IsActive() const{ return m_ActiveSelf && (m_Parent ? m_Parent->IsActive() : true); }

		void SetParent(GameObject& parent);
		GameObject* GetParent() const { return m_Parent; }
		std::vector<GameObject*>& GetChildren();

		template<typename T, typename ... Args>
		T& AddComponent(Args&&... args);

		template<typename T>
		T* GetComponent();

		bool RemoveComponent(Component& component);
	private:

		void UpdateActive();
		void OnActiveChange();
		void DetachParent();
		void MarkAsDestroy();

		/// <summary>
		/// 物体激活下才会执行，只执行一次
		/// </summary>
		void OnAwake();
		/// <summary>
		/// 物体激活下才会执行，只执行一次
		/// </summary>
		void OnStart();
		void OnEnable();
		void OnFixedUpdate();
		void OnUpdate();
		void OnLateUpdate();
		void OnDisable();
		void OnDestroy();
	public:


	private:
		std::string m_Name;
		int64_t m_ID;
		bool m_ActiveSelf = true;

		GameObject* m_Parent = nullptr;
		std::vector<GameObject*> m_Childrens;

		std::vector<std::shared_ptr<Component>> m_Components;

	private:
		//是否已经执行过OnAwake
		bool m_Awaked = false;
		bool m_Started = false;
		bool m_Destroyed = false;

		bool m_Active = false;
	};
}

