#pragma once
#include <SDL.h>
#include <memory>
#include <string>

class SDLApp
{
public:
	SDLApp();
	SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight);
	bool init();
	void run();
	void draw();
	void close();

	std::shared_ptr<SDL_Surface> loadBMP(std::string bmpMediaPath);

private:
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_screenSurface = nullptr;
};
