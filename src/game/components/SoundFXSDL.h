#pragma once
#include "../../engine/ecs/Component.h"
#include <SDL_mixer.h>
#include <string>
#include <memory>

struct SoundFXSDL : public Component
{
	SoundFXSDL(std::string path) : path(path) {};

	std::string path;
	std::shared_ptr<Mix_Chunk> soundFX;
};
