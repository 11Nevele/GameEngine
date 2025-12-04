#pragma once
#include "acpch.h"
#include <glm/glm.hpp>
struct AmbientLight
{
	glm::vec3 color;
	float intensity;
};

struct PointLight
{
	glm::vec3 color;
};

