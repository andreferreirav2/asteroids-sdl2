#pragma once
#include "../Types.h"
#include <SDL.h>
#include <memory>
#include <string>

struct LoadedTexture
{
	LoadedTexture(std::shared_ptr<SDL_Texture> texture, std::string path, uint2 dimentions) : texture(texture), path(path), dimentions(dimentions) {};

	std::shared_ptr<SDL_Texture> texture;
	std::string path;
	uint2 dimentions;
};