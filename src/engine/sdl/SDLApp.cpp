#include "SDLApp.h"
#include "SDLResourceDeleters.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
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
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
	{
		cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}
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

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cerr << "Warning: Linear texture filtering not enabled" << endl;
	}

	// Check for joysticks / controllers
	if (SDL_NumJoysticks() < 1 || !SDL_IsGameController(0))
	{
		cerr << "Warning: No joysticks connected" << endl;
	}
	else
	{
		m_controller = shared_ptr<SDL_GameController>(SDL_GameControllerOpen(0), SDLGameControllerDeleter());
		if (m_controller == nullptr)
		{
			cerr << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			//Get controller haptic device
			m_haptic = shared_ptr<SDL_Haptic>(SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(m_controller.get())), SDLHapticDeleter());
			if (m_haptic == NULL)
			{
				cerr << "Warning: Controller does not support haptics! SDL Error: " << SDL_GetError() << endl;
			}
			else
			{
				//Get initialize rumble
				if (SDL_HapticRumbleInit(m_haptic.get()) < 0)
				{
					cerr << "Warning: Unable to initialize rumble! SDL Error: " << SDL_GetError() << endl;
				}
			}
		}
	}

	// Init image library
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << IMG_GetError() << endl;
		return false;
	}

	return true;
}

void SDLApp::close()
{
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}