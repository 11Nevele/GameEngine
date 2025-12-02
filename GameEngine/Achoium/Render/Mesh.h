#pragma once
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace ac
{
	class Mesh
	{
	public:
		virtual VertexArray* GetVertexArray() = 0;
		virtual void BindTextures() = 0;
	};
}
