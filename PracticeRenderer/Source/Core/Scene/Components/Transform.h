#pragma once
#include "Core/Scene/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PR
{
	class Transform : public Component
	{
	public:
		Transform(GameObject& go);
		~Transform() = default;

		void SetParent(Transform& parent);

		const glm::vec3& GetLocalEulerAngles() const { return m_LocalRotation; }
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetLocalScale() const { return m_LocalScale; }
		/*const glm::qua<float>& GetLocalRotation();*/
		const glm::vec3& GetLocalRotation() const { return m_LocalRotation; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		/*const glm::qua<float>& GetRotation();*/
		const glm::vec3& GetRotation() const { return m_Rotation; }

		void SetLocalEulerAngles();
		void SetLocalPosition();
		void SetLocalScale();
		void SetLocalRotation();

		void SetPosition();
		void SetScale();
		void SetRotation();

		const glm::mat4& GetLocalToWorldMatrix() const { return m_LocalToWorldMatrix; }
		const glm::mat4& GetWorldToLocalMatrix() const { return m_WorldToLocalMatrix; }

		const glm::vec3& GetLocalForward();
		const glm::vec3& GetLocalRight();
		const glm::vec3& GetLocalUp();

		const glm::vec3& GetForward();
		const glm::vec3& GetRight();
		const glm::vec3& GetUp();

	private:
		void UpdateLoclaMatrix();
		void UpdateWorldMatrix();

	private:
		Transform* m_Parent = nullptr;

		glm::vec3 m_LocalPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LocalScale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_LocalRotation = { 0.0f, 0.0f, 0.0f };

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };

		glm::mat4 m_LocalToWorldMatrix = glm::mat4(1.0f);
		glm::mat4 m_WorldToLocalMatrix = glm::mat4(1.0f);

		glm::mat4 m_LocalToParentMatrix = glm::mat4(1.0f);
	};
}
