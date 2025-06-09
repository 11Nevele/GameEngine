#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
namespace ac
{
	class Transform
	{
	public:
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		Transform();
		glm::mat4 asMat4();
		void RotateAxis(glm::vec3, float angle);
		void RotateX(float angle);
		void RotateY(float angle);
		void RotateZ(float angle);
		void SetRotation(glm::vec3 eulerAngle);
		void RotateYaw(float angle);
		void RotatePitch(float angle);
		void RotateRoll(float angle);
	};
}
