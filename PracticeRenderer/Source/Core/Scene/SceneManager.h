#pragma once

#include "Core/Scene/Scene.h"

namespace PR
{
	class SceneManager
	{
	public:
		~SceneManager();

		void LoadScene(std::string scenePath);
		Scene* GetCurrentScene();
	private:
		Scene* m_currentScene = nullptr;
	};
}

