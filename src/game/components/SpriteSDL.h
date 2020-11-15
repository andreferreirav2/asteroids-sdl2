#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/Types.h"
#include <string>
#include <memory>

struct SDL_Texture;
struct LoadedTexture;

struct SpriteSDL : public Component
{
	SpriteSDL(std::string path, float rotationAngle = 0.0f, bool flipHorizontal = false, bool flipVertical = false, uint2 size = { 0, 0 }, rect cliping = { 0, 0, 0, 0 }) :
		path(path), rotationAngle(rotationAngle), flipHorizontal(flipHorizontal), flipVertical(flipVertical), size(size), cliping(cliping), loadedTexture(nullptr)
	{
	};

	std::string path;
	float rotationAngle;
	bool flipHorizontal;
	bool flipVertical;
	uint2 size;
	rect cliping;
	std::shared_ptr<LoadedTexture> loadedTexture;
};
