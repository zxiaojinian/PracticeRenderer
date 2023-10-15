#pragma once

#include "Core/Scene/Component.h"
#include "Core/Scene/Components/Camera.h"

namespace PR
{
	class CameraController : public Component
	{
	public:
		CameraController(GameObject& gameobject);

		//virtual void OnAwake() override;
		virtual void OnUpdate() override;

		virtual std::string GetName() override { return "CameraController"; }
	private:
		//Camera* m_Camera;

		float m_MoveSpeed = 1.0f;
		float m_RotateSpeed = 0.2f;

		bool m_StartDrag = true;
		float m_MouseLastPosX = 0.0f;
		float m_MouseLastPosY = 0.0f;
		//float m_Yaw = 90.0f;
		//float m_Pitch = 0.0f;
		float m_Yaw = 0.0f;
		float m_Pitch = 0.0f;
	};
}

