#pragma once

#include "Core/Scene/GameObject.h"

#include <string>

namespace PR
{
	class Scene
	{
	public:
		Scene() = default;
		~Scene();

		void OnFixedUpdate();
		void OnUpdate();
		void OnLateUpdate();

		void AddGameObject(GameObject& go);
		GameObject& CreateGameObject();
		GameObject& CreateGameObject(const std::string& name);
		bool DestroyGameObject(GameObject& go);

		std::vector<GameObject*>& GetGameObjecs();
		GameObject* FindGameObjectByName(const std::string& name);
		std::vector<GameObject*> FindGameObjectsByName(const std::string& name);
	private:
		std::vector<GameObject*> m_GameObjects;
	};
}


