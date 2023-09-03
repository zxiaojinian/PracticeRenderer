#pragma once

#include "Core/Scene/GameObject.h"
#include "Core/Scene/Components/Camera.h"
#include "Core/Scene/Components/Light.h"
#include "Core/Scene/Components/MeshRenderer.h"

#include <string>

namespace PR
{
	class Scene
	{
		friend class GameObject;
	public:
		Scene();
		~Scene();

		void OnFixedUpdate();
		void OnUpdate();
		void OnLateUpdate();

		GameObject& CreateGameObject();
		GameObject& CreateGameObject(const std::string& name);
		bool DestroyGameObject(GameObject& go);

		std::vector<GameObject*>& GetGameObjecs();
		GameObject* FindGameObjectByName(const std::string& name);
		std::vector<GameObject*> FindGameObjectsByName(const std::string& name);

		const std::vector<Camera*>& GetCameras() const { return m_Cameras; }
		const std::vector<Light*> GetLights() const { return m_Lights; }
		const std::vector<MeshRenderer*> GetMeshRenderers() const { return m_MeshRenderers; }

	private:
		void AddGameObject(GameObject& go);
		void OnComponentAdd(Component& component);
		void OnComponentRemove(Component& component);

	private:
		std::vector<GameObject*> m_GameObjects;

		std::vector<Camera*> m_Cameras;
		std::vector<Light*> m_Lights;
		std::vector<MeshRenderer*> m_MeshRenderers;
	};
}


