#include "pch.h"
#include "Scene.h"

#include "Core/Scene/GameObject.h"

#include "Core/Scene/Components/Transform.h"
#include "Core/Scene/Components/Camera.h"
#include "Core/Scene/Components/CameraController.h"
#include "Core/Asset/Resources.h"

namespace PR
{
	Scene::Scene()
	{
		auto mainCamera = new GameObject("MainCamera", this);
		mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<CameraController>();

		auto directionalLight = new GameObject("Directional Light", this);
		directionalLight->AddComponent<Light>();

		//std::string path = "Assets/Model/nanosuit/nanosuit.obj";
		//std::string path = "Assets/Model/Sponza/Sponza_Modular.FBX";
		//std::string path = "Assets/Model/sphere.fbx";
		std::string path = "Assets/Model/Sponza/sponza.fbx";
		Resources::Get().LoadModel(path, this);
	}

	Scene::~Scene()
	{
		for (auto go : m_GameObjects)
		{
			delete go;
		}
		m_GameObjects.clear();
	}

	void Scene::OnFixedUpdate()
	{
		for (auto go : m_GameObjects)
		{
			go->OnFixedUpdate();
		}
	}

	void Scene::OnUpdate()
	{
		for (auto go : m_GameObjects)
		{
			go->OnUpdate();
		}
	}

	void Scene::OnLateUpdate()
	{
		for (auto go : m_GameObjects)
		{
			go->OnLateUpdate();
		}
	}

	void Scene::AddGameObject(GameObject& go)
	{
		m_GameObjects.push_back(&go);
	}

	void Scene::OnComponentAdd(Component& component)
	{
		if (auto c = dynamic_cast<Camera*>(&component))
			m_Cameras.push_back(c);
		else if (auto c = dynamic_cast<Light*>(&component))
			m_Lights.push_back(c);
		else if (auto c = dynamic_cast<MeshRenderer*>(&component))
			m_MeshRenderers.push_back(c);
	}

	void Scene::OnComponentRemove(Component& component)
	{
		if (auto c = dynamic_cast<Camera*>(&component))
			m_Cameras.erase(std::remove(m_Cameras.begin(), m_Cameras.end(), c));
		else if (auto c = dynamic_cast<Light*>(&component))
			m_Lights.erase(std::remove(m_Lights.begin(), m_Lights.end(), c));
		else if (auto c = dynamic_cast<MeshRenderer*>(&component))
			m_MeshRenderers.erase(std::remove(m_MeshRenderers.begin(), m_MeshRenderers.end(), c));
	}

	bool Scene::DestroyGameObject(GameObject& go)
	{
		auto target = m_GameObjects.end();
		for (auto i = m_GameObjects.begin(); i != m_GameObjects.end(); ++i)
		{
			if (*i == &go)
			{
				target = i;
				break;
			}
		}
		if (target != m_GameObjects.end())
		{
			delete *target;
			m_GameObjects.erase(target);
			return true;
		}
		return false;
	}

	std::vector<GameObject*>& Scene::GetGameObjecs()
	{
		return m_GameObjects;
	}

	GameObject* Scene::FindGameObjectByName(const std::string& name)
	{
		auto result = std::find_if(m_GameObjects.begin(), m_GameObjects.end(), [name](GameObject* go)
			{
				return go->GetName() == name;
			});

		if (result != m_GameObjects.end())
			return *result;
		else
			return nullptr;
	}

	std::vector<GameObject*> Scene::FindGameObjectsByName(const std::string& name)
	{
		std::vector<GameObject*> goes;

		for (auto go : m_GameObjects)
		{
			if (go->GetName() == name)
				goes.push_back(go);
		}
		return goes;
	}
}
