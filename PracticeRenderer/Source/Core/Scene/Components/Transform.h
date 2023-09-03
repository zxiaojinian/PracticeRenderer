#pragma once
#include "Core/Scene/Component.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace PR
{
	enum class Space
	{
		Self,
		World
	};

	class Transform : public Component
	{
	public:
		Transform(GameObject& go);

		virtual std::string GetName() override { return "Transform"; }

		void SetParent(Transform* parent, bool worldPositionStays = true);

		glm::vec3 GetLocalEulerAngles() const;
		const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
		const glm::vec3& GetLocalScale() const { return m_LocalScale; }
		const glm::quat& GetLocalRotation() const { return m_LocalRotation; }

		glm::vec3 GetEulerAngles() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetScale() const { return m_Scale; }
		const glm::quat& GetRotation() const { return m_Rotation; }

		void SetLocalEulerAngles(const glm::vec3& angle);
		void SetLocalEulerAngles(float x, float y, float z);
		void SetLocalPosition(const glm::vec3& pos);
		void SetLocalScale(const glm::vec3& scale);
		void SetLocalRotation(const glm::quat& rotation);

		void SetEulerAngles(const glm::vec3& angle);
		void SetEulerAngles(float x, float y, float z);
		void SetPosition(const glm::vec3& pos);
		void SetScale(const glm::vec3& scale);
		void SetRotation(const glm::quat& rotation);

		const glm::mat4& GetLocalToWorldMatrix() const { return m_LocalToWorldMatrix; }
		glm::mat4 GetWorldToLocalMatrix() const;

		glm::vec3 GetLocalRight();
		glm::vec3 GetLocalUp();
		glm::vec3 GetLocalForward();

		glm::vec3 GetRight();
		glm::vec3 GetUp();
		glm::vec3 GetForward();

		void Translate(const glm::vec3& translation, Space relativeTo = Space::Self);
		void LookAt(const glm::vec3& target, const glm::vec3& worldUp = glm::vec3(0.0, 1.0f, 0.0f));
		void LookAt(const Transform& target, const glm::vec3& worldUp = glm::vec3(0.0, 1.0f, 0.0f));
	private:
		void ParentSpaceChange();
		void WorldSpaceChange();

		void UpdateLoclaMatrix();
		void UpdateWorldMatrix();

		void DecomposeMatrix(const glm::mat4& m, glm::vec3* pos, glm::quat* rotation, glm::vec3* scale);

	private:
		Transform* m_Parent = nullptr;
		std::vector<Transform*> m_Children;

		glm::vec3 m_LocalPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_LocalScale = { 1.0f, 1.0f, 1.0f };
		glm::quat m_LocalRotation = glm::quat();

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
		glm::quat m_Rotation = glm::quat();

		glm::mat4 m_LocalToWorldMatrix = glm::mat4(1.0f);
		glm::mat4 m_LocalToParentMatrix = glm::mat4(1.0f);
	};
}
