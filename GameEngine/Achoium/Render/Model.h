#pragma once
#include "Mesh.h"
#include "Buffer.h"
#include "VertexArray.h"
#include <vector>
namespace ac
{
	class Model
	{
	public:
		virtual std::vector<std::shared_ptr<Mesh>>& GetMeshes() = 0;
	};
}
