#pragma once

namespace PR
{
	class GameObject;

	class Component
	{
	public:
		Component(GameObject& gameobject);
		virtual ~Component();

		virtual void OnAwake() {}
		virtual void OnStart() {}
		virtual void OnEnable() {}
		virtual void OnFixedUpdate() {}
		virtual void OnUpdate() {}
		virtual void OnLateUpdate() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

	public:
		GameObject& gameobject;
	};
}

