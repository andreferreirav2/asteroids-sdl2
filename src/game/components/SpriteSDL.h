#pragma once
#include "../../engine/ecs/Component.h"
#include "../../engine/sdl/LoadedTexture.h"
#include "../../engine/Types.h"
#include <string>
#include <memory>

struct SDL_Texture;

struct SpriteSDL : public Component
{
	SpriteSDL(std::string path) : path(path), size({ 0, 0 }), cliping({ 0, 0, 0, 0 }), loadedTexture(nullptr) {};
	SpriteSDL(std::string path, uint2 size) : path(path), size(size), cliping({ 0, 0, 0, 0 }), loadedTexture(nullptr) {};
	SpriteSDL(std::string path, uint2 size, rect cliping) : path(path), size(size), cliping(cliping), loadedTexture(nullptr) {};

	std::string path;
	uint2 size;
	rect cliping;
	std::shared_ptr<LoadedTexture> loadedTexture;
};
