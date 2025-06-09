#include <acpch.h>
#include "Quaternion.h"

namespace ac
{
	Quaternion::Quaternion(const glm::vec4 v)
	{
		x = v.x, y = v.y, z = v.z, w = v.w;
	}
	Quaternion::Quaternion(float x, float y, float z, float w)
	{
		this->x = x, this->y = y, this->z = z, this->w = w;
	}
	Quaternion Quaternion::operator* (const Quaternion& other)
	{
		return Quaternion(
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w,
			w * other.w - x * other.x - y * other.y - z * other.z);
	}
	Quaternion Quaternion::operator - ()
	{
		return Quaternion(-x, -y, -z, w);
	}
	glm::mat4 Quaternion::asMat4()
	{
		glm::mat4 m;
		return m;
	}
}