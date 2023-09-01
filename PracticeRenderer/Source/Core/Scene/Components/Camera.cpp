#include "pch.h"
#include "Camera.h"
#include "Core/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PR
{
	Camera::Camera(GameObject& gameobject)
		: Component(gameobject), m_ProjectionMatrix(glm::perspective(glm::radians(45.0f), 1920.0f/1080.0f, 0.3f, 1000.0f))
	{
	}

	glm::u32vec4 Camera::GetPixelRect() const
	{
		return glm::u32vec4(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
	}

	const glm::mat4 Camera::GetViewMatrix() const
	{
		return m_Transform.GetWorldToLocalMatrix();
	}
	const glm::mat4 Camera::GetViewProjectionMatrix() const
	{
		return m_ProjectionMatrix * GetViewMatrix();
	}
}
