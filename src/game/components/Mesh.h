#pragma once
#include "../../engine/ecs/Component.h"
#include <string>

struct Mesh : public Component
{
	Mesh(std::string path, float scale = 1.0f, float rotation = 0.0f) :
		path(path),
		scale(scale),
		rotation(rotation)
	{
	};

	std::string path;
	float rotation;
	float scale;
};
