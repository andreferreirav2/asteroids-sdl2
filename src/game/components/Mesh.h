#pragma once
#include "../../engine/ecs/Component.h"
#include <string>

struct Mesh : public Component
{
	Mesh(std::string path) :
		path(path)
	{
	};

	std::string path;
};
