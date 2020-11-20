#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/Types.h"
#include <string>

struct Mesh : public Component
{
	Mesh(std::string path, float scale = 1.0f, float rotation = 0.0f, float3 colorDiffuse = { 1.0f, 1.0f, 1.0f }, float emissiveness = 0.0f) :
		path(path),
		scale(scale),
		rotation(rotation),
		colorDiffuse(colorDiffuse),
		emissiveness(emissiveness)
	{
	};

	std::string path;
	float rotation;
	float scale;
	float3 colorDiffuse;
	float emissiveness;
};
