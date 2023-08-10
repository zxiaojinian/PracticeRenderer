#pragma once

namespace PR
{
	class GameObject;
	class Transform;

	class Component
	{
	public:
		Component(GameObject& gameobject);
		virtual ~Component();
		Component(const Component& c) = delete;

		GameObject& GetGameObject() const { return m_Gameobject; }
		Transform& GetTransform() const { return m_Transform; }

		bool GetDisallowMultipleComponent() { return m_DisallowMultipleComponent; }

	public:
		virtual void OnAwake() {}
		virtual void OnEnable() {}
		virtual void OnStart() {}
		virtual void OnFixedUpdate() {}
		virtual void OnUpdate() {}
		virtual void OnLateUpdate() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}


	protected:
		bool m_DisallowMultipleComponent = false;
		GameObject& m_Gameobject;
		Transform& m_Transform;
	};
}

