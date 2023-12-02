#include "pch.h"
#include "Scene.h"

#include "Core/Scene/GameObject.h"

#include "Core/Scene/Components/Transform.h"
#include "Core/Scene/Components/Camera.h"
#include "Core/Scene/Components/CameraController.h"
#include "Core/Asset/Resources.h"

#include "Core/Util/Random.h"

namespace PR
{
	Scene::Scene()
	{
		auto mainCamera = new GameObject("MainCamera", this);
		mainCamera->AddComponent<Camera>();
		mainCamera->AddComponent<CameraController>();

		auto directionalLight = new GameObject("Directional Light", this);
		directionalLight->AddComponent<Light>();

		//Lights
		Random::SetSeed(1);
		float rangeX = 2.0f;
		float rangeY = 7.0f;
		float rangeZ = 11.0f;
		uint32_t numPointLights = 256;
		for (uint32_t i = 0; i < numPointLights; i++)
		{
			auto go = new GameObject("Pont Light" + std::to_string(i), this);
			auto& light = go->AddComponent<Light>();
			light.Type = LightType::Point;
			light.LightColor = { Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), 1.0};
			light.Intensity = Random::Range(2.0f, 5.0f);
			light.LightRange = Random::Range(1.0f, 3.0f);

			glm::vec3 position = { Random::Range(-rangeX, rangeX), Random::Range(0.5f, rangeY) , Random::Range(-rangeZ, rangeZ) };
			go->GetTransform().SetPosition(position);
		}

		uint32_t numSpotLights = 64;
		for (uint32_t i = 0; i < numSpotLights; i++)
		{
			auto go = new GameObject("Pont Light" + std::to_string(i), this);
			auto& light = go->AddComponent<Light>();
			light.Type = LightType::Spot;
			light.LightColor = { Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), Random::Range(0.0f, 1.0f), 1.0 };
			light.Intensity = Random::Range(5.0f, 10.0f);
			light.LightRange = Random::Range(5.0f, 10.0f);
			light.InnerSpotAngle = Random::Range(30.0f, 90.0f);
			light.SpotAngle = light.InnerSpotAngle + Random::Range(20.0f, 30.0f);
			glm::vec3 position = { Random::Range(-rangeX, rangeX), Random::Range(0.5f, rangeY) , Random::Range(-rangeZ, rangeZ) };
			go->GetTransform().SetPosition(position);
			go->GetTransform().SetEulerAngles(glm::vec3(Random::Range(70.0f, 110.0f), 0.0f, Random::Range(-20.0f, 20.0f)));
		}

		std::string path = "Assets/Model/Sponza/Scene_Sponza.fbx";
		Resources::Get().LoadModel(path, this);

		//auto* sphere = Resources::Get().LoadModel("Assets/Model/Sponza/sphere.fbx", this);
		//if (sphere)
		//{
		//	sphere->GetTransform().Translate(glm::vec3(.0F, 1.0F, 0.0F), Space::World);
		//	//sphere->GetTransform().SetScale(glm::vec3(0.01f));
		//}
		
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
