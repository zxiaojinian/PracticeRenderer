#pragma once

#include "Core/Scene/Scene.h"

namespace PR
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		static SceneManager& Get() { return *s_Instance; }

		void LoadScene(std::string scenePath);
		Scene* GetCurrentScene();
	private:
		Scene* m_currentScene = nullptr;

		static SceneManager* s_Instance;
	};
}

