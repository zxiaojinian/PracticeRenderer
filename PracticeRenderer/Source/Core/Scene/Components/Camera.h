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
		const glm::mat4 GetViewMatrix() const;
		const glm::mat4 GetViewProjectionMatrix() const;

		virtual std::string GetName() override { return "Camera"; }

	public:
		uint16_t RendererIndex = 0;

		float m_Fov = 60.0f;
		float m_NearPlane = 0.1f;
		float m_FarPlane = 1000.0f;
		glm::mat4 m_ProjectionMatrix;
		//glm::mat4 m_ViewProjectionMatrix;
	};
}

