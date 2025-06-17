#include <acpch.h>
#include "Transform.h"
#include "util.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace ac
{
	glm::mat4 Transform::asMat4(bool invPosition)
	{
        glm::mat4 transform = glm::mat4(1.0f);

		if (!invPosition)
		{
			transform = glm::translate(transform, position);
			transform *= glm::toMat4(rotation);

			transform = glm::scale(transform, scale);
		}
		else
		{
			transform = glm::translate(transform, -position);

			transform *= glm::toMat4(glm::inverse(rotation));

			transform = glm::scale(transform, glm::vec3(1.0f) / scale);
		}

		

        return transform;

	}
	Transform::Transform() :position(0,0,0), rotation(glm::vec3(0,0,0)), scale(1, 1, 1)
	{
	}
	void Transform::RotateAxis(glm::vec3 axis, float angle)
	{
		// Create a quaternion representing the rotation
		glm::quat q = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

		// Combine the new rotation with the existing rotation
		rotation = q * rotation;
	}

	void Transform::RotateX(float angle)
	{
		RotateAxis(glm::vec3(1.0f, 0.0f, 0.0f), angle);
	}

	void Transform::RotateY(float angle)
	{
		RotateAxis(glm::vec3(0.0f, 1.0f, 0.0f), angle);
	}

	void Transform::RotateZ(float angle)
	{
		RotateAxis(glm::vec3(0.0f, 0.0f, 1.0f), angle);
	}

	void Transform::SetRotation(glm::vec3 eulerAngle)
	{
		// Convert Euler angles (in radians) to a quaternion
		rotation = glm::quat(glm::radians(eulerAngle));
	}

	void Transform::RotateYaw(float angle)
	{
		// Rotate around the Y-axis (yaw) relative to the current rotation
		glm::quat yawQuat = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		rotation = yawQuat * rotation;
	}

	void Transform::RotatePitch(float angle)
	{
		// Rotate around the X-axis (pitch) relative to the current rotation
		glm::quat pitchQuat = glm::angleAxis(glm::radians(angle), glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = pitchQuat * rotation;
	}

	void Transform::RotateRoll(float angle)
	{
		// Rotate around the Z-axis (roll) relative to the current rotation
		glm::quat rollQuat = glm::angleAxis(glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = rollQuat * rotation;
	}
}