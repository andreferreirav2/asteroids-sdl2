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

std::shared_ptr<Inputs> SDLApp::parseInputs()
{
	SDL_Event e;
	set<Key> keys = {};
	while (SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_QUIT)
		{
			keys.insert(Key::QUIT);
		}
	}

	/*
	Uint8 const* keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDLK_ESCAPE])
		keys.insert(Key::QUIT);

	if (keystates[SDLK_UP])
		keys.insert(Key::KEY_UP);
	if (keystates[SDLK_DOWN])
		keys.insert(Key::KEY_DOWN);
	if (keystates[SDLK_LEFT])
		keys.insert(Key::KEY_LEFT);
	if (keystates[SDLK_RIGHT])
		keys.insert(Key::KEY_RIGHT);
	if (keystates[SDLK_SPACE])
		keys.insert(Key::KEY_SPACE);
	*/

	m_inputs->update(keys);
	return m_inputs;
}

void SDLApp::clear(colorR8G8B8A8 color)
{
	setColor(color);
	SDL_RenderClear(m_renderer.get());
}

void SDLApp::drawTexture(std::shared_ptr<SDL_Texture> texture)
{
	SDL_RenderCopy(m_renderer.get(), texture.get(), NULL, NULL);
}

void SDLApp::drawDot(uint2 pos, colorR8G8B8A8 color)
{
	setColor(color);
	SDL_RenderDrawPoint(m_renderer.get(), pos.x, pos.y);
}

void SDLApp::drawLine(uint2 start, uint2 end, colorR8G8B8A8 color)
{
	setColor(color);
	SDL_RenderDrawLine(m_renderer.get(), start.x, start.y, end.x, end.y);
}

void SDLApp::drawRect(uint2 start, uint2 size, colorR8G8B8A8 color, bool filled)
{
	SDL_Rect rect = { static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(size.x), static_cast<int>(size.y) };
	setColor(color);
	if (filled)
	{
		SDL_RenderFillRect(m_renderer.get(), &rect);
	}
	else
	{
		SDL_RenderDrawRect(m_renderer.get(), &rect);
	}

}

void SDLApp::present()
{
	SDL_RenderPresent(m_renderer.get());
}

shared_ptr<SDL_Texture> SDLApp::loadTexture(string imagePath)
{
	shared_ptr<SDL_Texture> texture = nullptr;
	shared_ptr<SDL_Surface> loadedSurface = shared_ptr<SDL_Surface>(IMG_Load(imagePath.c_str()), SDLSurfaceDeleter());
	if (loadedSurface == NULL)
	{
		cerr << "Unable to load image " << imagePath << "! SDL Error: " << SDL_GetError() << endl;
		return nullptr;
	}

	texture = shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(m_renderer.get(), loadedSurface.get()), SDLTextureDeleter());
	if (texture == NULL)
	{
		cerr << "Unable to create texture from  " << imagePath << "! SDL Error: " << SDL_GetError() << endl;
		return nullptr;
	}
	return texture;
}

void SDLApp::close()
{
	IMG_Quit();
	SDL_Quit();
}

void SDLApp::setColor(colorR8G8B8A8 color)
{
	SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
}
