#pragma once

#include <glm/glm.hpp>

namespace PR
{
	class Camera
	{
	public:
		Camera(const glm::mat4& projection)
			: m_Projection(projection)
		{}

		Camera() = default;
		virtual ~Camera() = default;
		const glm::mat4& GetProjectionMatrix() const { return m_Projection; }

	protected:
		glm::mat4 m_Projection = glm::mat4(1.0f);
	};
}
