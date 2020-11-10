#pragma once
#include <SDL.h>

struct SDLWindowDeleter
{
	void operator()(SDL_Window* window)
	{
		SDL_DestroyWindow(window);
	}
};

struct SDLGameControllerDeleter
{
	void operator()(SDL_GameController* controller)
	{
		SDL_GameControllerClose(controller);
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