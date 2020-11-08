#include "SDLApp.h"
#include "../input/Inputs.h"
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <memory>
#include <set>

using std::cerr;
using std::set;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;


SDLApp::SDLApp() :
	m_screenWidth(640),
	m_screenHeight(480),
	m_window(nullptr),
	m_screenSurface(nullptr)
{
}

SDLApp::SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight) :
	m_screenWidth(m_screenWidth),
	m_screenHeight(m_screenHeight),
	m_window(nullptr),
	m_screenSurface(nullptr)
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
		m_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
		if (m_window == nullptr)
		{
			cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			return false;
		}

		int imgFlags = IMG_INIT_PNG;
		if (!(IMG_Init(imgFlags) & imgFlags))
		{
			cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
			return false;
		}
	}
	return true;
}

std::shared_ptr<Inputs> SDLApp::parseInputs()
{
	SDL_Event e;
	set<Key> keys = {};
	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			keys.insert(Key::QUIT);
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				keys.insert(Key::KEY_UP);
				break;
			case SDLK_DOWN:
				keys.insert(Key::KEY_DOWN);
				break;
			case SDLK_LEFT:
				keys.insert(Key::KEY_LEFT);
				break;
			case SDLK_RIGHT:
				keys.insert(Key::KEY_RIGHT);
				break;
			case SDLK_SPACE:
				keys.insert(Key::KEY_SPACE);
				break;
			default:
				cerr << "other? " << e.key.keysym.sym << endl;
				break;
			}
			break;
		default:
			break;
		}
	}
	m_inputs->update(keys);
	return m_inputs;
}

void SDLApp::fillScreen(Uint8 r, Uint8 g, Uint8 b)
{
	m_screenSurface = SDL_GetWindowSurface(m_window);
	SDL_FillRect(m_screenSurface, nullptr, SDL_MapRGB(m_screenSurface->format, 0xFF, 0xFF, 0xFF));
}

void SDLApp::drawSurface(std::shared_ptr<SDL_Surface> surface)
{
	SDL_BlitSurface(surface.get(), NULL, m_screenSurface, NULL);
}

void SDLApp::flush()
{
	SDL_UpdateWindowSurface(m_window);
}

shared_ptr<SDL_Surface> SDLApp::loadBMP(string bmpMediaPath)
{
	shared_ptr<SDL_Surface> bmpMedia = shared_ptr<SDL_Surface>(SDL_LoadBMP(bmpMediaPath.c_str()), [](SDL_Surface* s) { SDL_FreeSurface(s); });
	if (bmpMedia == NULL)
	{
		cerr << "Unable to load image " << bmpMediaPath << "! SDL Error: " << SDL_GetError() << endl;
	}
	return bmpMedia;
}

void SDLApp::close()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}