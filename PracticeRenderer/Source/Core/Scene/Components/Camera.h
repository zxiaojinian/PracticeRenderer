#pragma once

#include "Core/Scene/Component.h"

#include<glm/glm.hpp>

namespace PR
{
	class Camera : public Component
	{
	public:
		Camera(GameObject& gameobject);
		glm::u32vec4 GetPixelRect() const;

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetInvProjectionMatrix() const { return m_InvProjectionMatrix; }
		const glm::mat4 GetViewMatrix() const;
		const glm::mat4 GetInvViewMatrix() const;
		const glm::mat4 GetViewProjectionMatrix() const;

		float GetFov() { return m_Fov; }
		float GetNearPlane() { return m_NearPlane; }
		float GetFarPlane() { return m_FarPlane; }

		virtual std::string GetName() override { return "Camera"; }

	public:
		uint16_t RendererIndex = 0;

	private:	
		float m_Fov = 60.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 1000.0f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_InvProjectionMatrix;
		//glm::mat4 m_ViewProjectionMatrix;
	};
}

