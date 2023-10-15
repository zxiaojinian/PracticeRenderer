#include "pch.h"
#include "Camera.h"

#include "Core/Application.h"
#include "Core/Scene/Components/Transform.h"
#include "Core/Application.h"

#include <glm/gtc/matrix_transform.hpp>

namespace PR
{
	Camera::Camera(GameObject& gameobject)
		: Component(gameobject)
	{
		float aspect = (float)Application::Get().GetWindow().GetWidth() / Application::Get().GetWindow().GetHeight();
		m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), aspect, m_NearPlane, m_FarPlane);
		m_Transform.SetPosition(glm::vec3(0.0f, 2.0f, 10.0f));
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
