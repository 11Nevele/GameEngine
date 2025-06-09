#pragma once
#include <glm/glm.hpp>
namespace ac
{
	class Quaternion
	{
	public:
		float x, y, z, w;
		Quaternion(const glm::vec4 v);
		Quaternion(float x, float y, float z, float w);
		Quaternion operator* (const Quaternion& other);
		Quaternion operator - ();
		glm::mat4 asMat4();
	};
}