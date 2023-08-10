#include "pch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace PR
{
	Transform::Transform(GameObject& go)
		: Component(go)
	{
		m_DisallowMultipleComponent = true;
	}

	void Transform::SetParent(Transform& parent)
	{
	}

	glm::vec3 Transform::GetLocalEulerAngles() const
	{
		return glm::eulerAngles(m_LocalRotation);
	}

	glm::vec3 Transform::GetEulerAngles() const
	{
		return glm::eulerAngles(m_Rotation);
	}

	void Transform::SetLocalEulerAngles(const glm::vec3& angle)
	{
		m_LocalRotation = glm::qua<float>(glm::radians(angle));
		OnLocalChange();
	}

	void Transform::SetLocalPosition(const glm::vec3& pos)
	{
		m_LocalPosition = pos;
		OnLocalChange();
	}

	void Transform::SetLocalScale(const glm::vec3& scale)
	{
		m_LocalScale = scale;
		OnLocalChange();
	}

	void Transform::SetLocalRotation(const glm::qua<float>& rotation)
	{
		m_LocalRotation = rotation;
		OnLocalChange();
	}

	void Transform::SetEulerAngles(const glm::vec3& angle)
	{
		m_Rotation = glm::qua<float>(glm::radians(angle));
		OnWorldChange();
	}

	void Transform::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		OnWorldChange();
	}

	void Transform::SetScale(const glm::vec3& scale)
	{
		m_Scale = scale;
		OnWorldChange();
	}

	void Transform::SetRotation(const glm::qua<float>& rotation)
	{
		m_Rotation = rotation;
		OnWorldChange();
	}


	glm::mat4 Transform::GetWorldToLocalMatrix() const
	{
		return glm::inverse(m_LocalToWorldMatrix);
	}

	glm::vec3 Transform::GetLocalForward()
	{
		return glm::vec3();
	}

	glm::vec3 Transform::GetLocalRight()
	{
		return glm::vec3();
	}

	glm::vec3 Transform::GetLocalUp()
	{
		return glm::vec3();
	}

	glm::vec3 Transform::GetForward()
	{
		return glm::vec3();
	}

	glm::vec3 Transform::GetRight()
	{
		return glm::vec3();
	}

	glm::vec3 Transform::GetUp()
	{
		return glm::vec3();
	}


	void Transform::OnLocalChange()
	{
		m_LocalToParentMatrix = glm::translate(glm::mat4(1.0f), m_LocalPosition) * glm::mat4_cast(m_LocalRotation) * glm::scale(glm::mat4(1.0f), m_LocalScale);
		UpdateWorldMatrix();
	}

	void Transform::OnWorldChange()
	{
		m_LocalToWorldMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::mat4_cast(m_Rotation) * glm::scale(glm::mat4(1.0f), m_Scale);
		UpdateLoclaMatrix();
	}

	void Transform::UpdateLoclaMatrix()
	{
		if (m_Parent)
		{
			m_LocalToParentMatrix = glm::inverse(m_Parent->m_LocalToWorldMatrix) * m_LocalToWorldMatrix;
		}
		else
		{
			m_LocalToParentMatrix = m_LocalToWorldMatrix;
			m_LocalPosition = m_Position;
			m_LocalRotation = m_Rotation;
			m_LocalScale = m_Scale;
		}
	}

	void Transform::UpdateWorldMatrix()
	{
		if (m_Parent)
		{
			m_LocalToWorldMatrix = m_Parent->m_LocalToWorldMatrix * m_LocalToParentMatrix;
			DecomposeMatrix(m_LocalToWorldMatrix, m_Position, m_Rotation, m_Scale);
		}
		else
		{
			m_LocalToWorldMatrix = m_LocalToParentMatrix;
			m_Position = m_LocalPosition;
			m_Rotation = m_LocalRotation;
			m_Scale = m_LocalScale;
		}
		//TODO更新子物体transform
	}

	void Transform::DecomposeMatrix(const glm::mat4& m, glm::vec3& pos, glm::quat& rotation, glm::vec3& scale)
	{
		pos = m[3];

		for (int i = 0; i < 3; i++)
			scale[i] = glm::length(glm::vec3(m[i]));

		const glm::mat3 rotationMatrix(
			scale[0] ? glm::vec3(m[0]) : glm::vec3(m[0]) / scale[0],
			scale[1] ? glm::vec3(m[1]) : glm::vec3(m[1]) / scale[1],
			scale[2] ? glm::vec3(m[2]) : glm::vec3(m[2]) / scale[2]);
		rotation = glm::quat_cast(rotationMatrix);
	}

}
