#include "SDLApp.h"
#include "SDLResourceDeleters.h"
#include <SDL_image.h>
#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::shared_ptr;


SDLApp::SDLApp() :
	m_screenWidth(640),
	m_screenHeight(480)
{
}

SDLApp::SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight) :
	m_screenWidth(m_screenWidth),
	m_screenHeight(m_screenHeight)
{
}

bool SDLApp::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
	else
	{
		// Create Window
		m_window = shared_ptr<SDL_Window>(
			SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN),
			SDLWindowDeleter()
			);
		if (m_window == nullptr)
		{
			cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			return false;
		}

		// Create texture renderer
		m_renderer = shared_ptr<SDL_Renderer>(
			SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED),
			SDLRendererDeleter());
		if (m_renderer == nullptr)
		{
			cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << endl;
			return false;
		}
		SDL_SetRenderDrawColor(m_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

		// Init image library
		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
			return false;
		}
	}
	return true;
}

void SDLApp::close()
{
	IMG_Quit();
	SDL_Quit();
}