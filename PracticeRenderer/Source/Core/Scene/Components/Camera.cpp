#include "pch.h"
#include "Camera.h"
#include "Core/Application.h"

namespace PR
{
	Camera::Camera(GameObject& gameobject)
		: Component(gameobject)
	{
	}
	glm::vec4 Camera::GetPixelRect() const
	{
		return glm::vec4(0.0f, 0.0f, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}
}
