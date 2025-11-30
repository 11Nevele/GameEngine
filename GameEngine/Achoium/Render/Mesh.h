#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace ac
{
	class Mesh
	{
	public:
		virtual VertexArray* GetVirtexArray() = 0;
	};
}
