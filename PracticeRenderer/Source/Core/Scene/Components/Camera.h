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

		glm::mat4 m_ProjectionMatrix;
		//glm::mat4 m_ViewProjectionMatrix;
	};
}

