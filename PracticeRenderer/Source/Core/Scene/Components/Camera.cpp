#include "pch.h"
#include "Camera.h"
#include "Core/Application.h"

namespace PR
{
	Camera::Camera(GameObject& gameobject)
		: Component(gameobject)
	{
	}
	glm::u32vec4 Camera::GetPixelRect() const
	{
		return glm::u32vec4(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}
}
