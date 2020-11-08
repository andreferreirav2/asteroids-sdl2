#pragma once
#include "../input/Inputs.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <set>

class SDLApp
{
public:
	SDLApp();
	SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight);
	bool init();
	std::shared_ptr<Inputs> parseInputs();
	void fillScreen(Uint8 r, Uint8 g, Uint8 b);
	void drawSurface(std::shared_ptr<SDL_Surface> surface);
	void flush();
	void close();

	std::shared_ptr<SDL_Surface> loadBMP(std::string bmpMediaPath);

private:
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_screenSurface = nullptr;
	std::shared_ptr<Inputs> m_inputs = std::make_shared<Inputs>();
};
