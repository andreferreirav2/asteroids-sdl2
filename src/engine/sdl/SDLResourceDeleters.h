#pragma once
#include <SDL.h>
#include <SDL_mixer.h>

struct SDLWindowDeleter
{
	void operator()(SDL_Window* window)
	{
		SDL_DestroyWindow(window);
	}
};

struct TTFFontDeleter
{
	void operator()(TTF_Font* font)
	{
		TTF_CloseFont(font);
	}
};

struct SDLGameControllerDeleter
{
	void operator()(SDL_GameController* controller)
	{
		SDL_GameControllerClose(controller);
	}
};

struct SDLHapticDeleter
{
	void operator()(SDL_Haptic* haptic)
	{
		SDL_HapticClose(haptic);
	}
};

struct MixMusicDeleter
{
	void operator()(Mix_Music* music)
	{
		Mix_FreeMusic(music);
	}
};

struct MixChunkDeleter
{
	void operator()(Mix_Chunk* chunk)
	{
		Mix_FreeChunk(chunk);
	}
};

struct SDLRendererDeleter
{
	void operator()(SDL_Renderer* renderer)
	{
		SDL_DestroyRenderer(renderer);
	}
};

struct SDLTextureDeleter
{
	void operator()(SDL_Texture* texture)
	{
		SDL_DestroyTexture(texture);
	}
};

struct SDLSurfaceDeleter
{
	void operator()(SDL_Surface* surface)
	{
		SDL_FreeSurface(surface);
	}
};