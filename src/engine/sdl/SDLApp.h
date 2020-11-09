#pragma once
#include "../input/Inputs.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <set>
#include "../Types.h"

class SDLApp
{
public:
	SDLApp();
	SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight);
	bool init();
	unsigned int getScreenWidth() { return m_screenWidth; }
	unsigned int getScreenHeigth() { return m_screenHeight; }

	std::shared_ptr<Inputs> parseInputs();
	std::shared_ptr<SDL_Texture> loadTexture(std::string imagePath);

	void clear(colorR8G8B8A8 color);
	void drawTexture(std::shared_ptr<SDL_Texture> texture);
	void drawDot(uint2 pos, colorR8G8B8A8 color);
	void drawLine(uint2 start, uint2 end, colorR8G8B8A8 color);
	void drawRect(uint2 start, uint2 size, colorR8G8B8A8 color, bool filled);
	void present();
	void close();


private:
	void setColor(colorR8G8B8A8 color);

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
	std::shared_ptr<SDL_Window> m_window = nullptr;
	std::shared_ptr<SDL_Renderer> m_renderer = nullptr;
	std::shared_ptr<Inputs> m_inputs = std::make_shared<Inputs>();
};

struct SDLWindowDeleter
{
	void operator()(SDL_Window* window)
	{
		SDL_DestroyWindow(window);
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