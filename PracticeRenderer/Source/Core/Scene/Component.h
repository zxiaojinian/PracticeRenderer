#pragma once

namespace PR
{
	class GameObject;

	class Component
	{
	public:
		Component(GameObject& gameobject);
		virtual ~Component();
		Component(const Component& c) = delete;

		const GameObject& GetGameObject() const { return m_Gameobject; }
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
	};
}

