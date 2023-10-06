#include "pch.h"
#include "CameraController.h"

#include "Core/Input/Input.h"
#include "Core/Scene/Components/Transform.h"
#include "Core/Time/Time.h"
#include "Core/Scene/GameObject.h"

#include <glm/glm.hpp>

namespace PR
{
	CameraController::CameraController(GameObject& gameobject)
		: Component(gameobject)
	{
	}

	//void CameraController::OnAwake()
	//{
	//	m_Camera = m_Gameobject.GetComponent<Camera>();
	//}

	void CameraController::OnUpdate()
	{
		if (Input::IsMouseButtonPressed(MouseCode::ButtonRight))
		{
			//rotate
			if (m_StartDrag)
			{
				m_StartDrag = false;

				m_MouseLastPosX = Input::GetMouseX();
				m_MouseLastPosY = Input::GetMouseY();
			}

			float xOffset = Input::GetMouseX() - m_MouseLastPosX;
			float yOffset = m_MouseLastPosY - Input::GetMouseY();

			m_MouseLastPosX = Input::GetMouseX();
			m_MouseLastPosY = Input::GetMouseY();

			//m_Yaw += xOffset * m_RotateSpeed;
			//m_Pitch -= yOffset * m_RotateSpeed;

			//glm::vec3 front;
			//front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			//front.y = sin(glm::radians(m_Pitch));
			//front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			//front = glm::normalize(front);
			//m_Transform.LookAt(m_Transform.GetPosition() + front);

			m_Yaw -= xOffset * m_RotateSpeed;
			m_Pitch += yOffset * m_RotateSpeed;
			m_Transform.SetEulerAngles(glm::vec3(m_Pitch, m_Yaw, 0.0f));
		}
		else
		{
			m_StartDrag = true;
		}

		//move
		glm::vec3 dir = glm::vec3(0.0f);
		if (Input::IsKeyPressed(KeyCode::W))
		{
			dir.z = -1.0f;
		}
		else if (Input::IsKeyPressed(KeyCode::S))
		{
			dir.z = 1.0f;
		}

		if (Input::IsKeyPressed(KeyCode::A))
		{
			dir.x = -1.0f;
		}
		else if (Input::IsKeyPressed(KeyCode::D))
		{
			dir.x = 1.0f;
		}

		if (Input::IsKeyPressed(KeyCode::E))
		{
			dir.y = 1.0f;
		}
		else if (Input::IsKeyPressed(KeyCode::Q))
		{
			dir.y = -1.0f;
		}

		float speedFactor = 1.0f;
		if (Input::IsKeyPressed(KeyCode::LeftShift))
			speedFactor = 20.0f;

		if (dir.x || dir.y || dir.z)
		{
			m_Transform.Translate(glm::normalize(dir) * Time::DletaTime() * m_MoveSpeed * speedFactor, Space::Self);
		}
	}
}
