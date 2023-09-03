#include "pch.h"
#include "SceneManager.h"

namespace PR
{
	SceneManager* SceneManager::s_Instance = nullptr;

	SceneManager::SceneManager()
	{
		s_Instance = this;
		LoadScene("");
	}

	SceneManager::~SceneManager()
	{
		if (m_currentScene)
		{
			delete m_currentScene;
			m_currentScene = nullptr;
		}			
	}

	void SceneManager::LoadScene(std::string scenePath)
	{
		m_currentScene = new Scene();
	}

	Scene* SceneManager::GetCurrentScene()
	{
		return m_currentScene;
	}
}