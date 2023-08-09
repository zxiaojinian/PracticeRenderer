#include "pch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

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

	void Transform::SetLocalEulerAngles()
	{
	}

	void Transform::SetLocalPosition()
	{
	}

	void Transform::SetLocalScale()
	{
	}

	void Transform::SetLocalRotation()
	{
	}

	void Transform::SetPosition()
	{
	}

	void Transform::SetScale()
	{
	}

	void Transform::SetRotation()
	{
	}

	const glm::vec3& Transform::GetLocalForward()
	{
		// TODO: 在此处插入 return 语句
	}

	const glm::vec3& Transform::GetLocalRight()
	{
		// TODO: 在此处插入 return 语句
	}

	const glm::vec3& Transform::GetLocalUp()
	{
		// TODO: 在此处插入 return 语句
	}

	const glm::vec3& Transform::GetForward()
	{
		// TODO: 在此处插入 return 语句
	}

	const glm::vec3& Transform::GetRight()
	{
		// TODO: 在此处插入 return 语句
	}

	const glm::vec3& Transform::GetUp()
	{
		// TODO: 在此处插入 return 语句
	}

	void Transform::UpdateLoclaMatrix()
	{
		//TODO 替换为四元数
		const glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), glm::radians(m_LocalRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		const glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), glm::radians(m_LocalRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), glm::radians(m_LocalRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		// Y * X * Z
		const glm::mat4 roationMatrix = rotateY * rotateX * rotateZ;

		m_LocalToParentMatrix = glm::translate(glm::mat4(1.0f), m_LocalPosition) * roationMatrix * glm::scale(glm::mat4(1.0f), m_LocalScale);
	}

	void Transform::UpdateWorldMatrix()
	{
		if (m_Parent)
		{
			m_LocalToWorldMatrix = m_Parent->m_LocalToWorldMatrix * m_LocalToParentMatrix;
		}
		else
		{
			m_LocalToWorldMatrix = m_LocalToParentMatrix;
		}
	}

}
