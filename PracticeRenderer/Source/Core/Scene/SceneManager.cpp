#include "pch.h"
#include "SceneManager.h"

namespace PR
{
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
		if (!m_currentScene)
		{
			LoadScene("");

		}
		return m_currentScene;
	}
}