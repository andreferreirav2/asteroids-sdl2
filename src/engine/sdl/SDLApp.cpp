#include "SDLApp.h"
#include "SDLResourceDeleters.h"
#include <iostream>
#include <memory>

#define LOG std::cerr

using std::shared_ptr;


SDLApp::SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight, bool opengl, std::string vertexShaderPath, std::string fragmentShaderPath) :
	m_screenWidth(m_screenWidth),
	m_screenHeight(m_screenHeight),
	m_opengl(opengl),
	m_vertexShaderPath(vertexShaderPath),
	m_fragmentShaderPath(fragmentShaderPath)
{
}

bool SDLApp::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
	{
		LOG << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	// Init OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create Window
	m_window = shared_ptr<SDL_Window>(
		SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL),
		SDLWindowDeleter()
		);
	if (m_window == nullptr)
	{
		LOG << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Create context
	m_glContext = SDL_GL_CreateContext(m_window.get());
	if (m_glContext == NULL)
	{
		LOG << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		LOG << "Error initializing GLEW: " << glewGetErrorString(glewError) << std::endl;
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		LOG << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
	}

	//Initialize OpenGL
	if (!initGL())
	{
		LOG << "Unable to initialize OpenGL!" << std::endl;
		return false;
	}

	// Create texture renderer
	m_renderer = shared_ptr<SDL_Renderer>(
		SDL_CreateRenderer(m_window.get(), -1, SDL_RENDERER_ACCELERATED),
		SDLRendererDeleter());
	if (m_renderer == nullptr)
	{
		LOG << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
		return false;
	}
	SDL_SetRenderDrawColor(m_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);

	// Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		LOG << "Warning: Linear texture filtering not enabled" << std::endl;
	}

	// Check for joysticks / controllers
	if (SDL_NumJoysticks() < 1 || !SDL_IsGameController(0))
	{
		LOG << "Warning: No joysticks connected" << std::endl;
	}
	else
	{
		m_controller = shared_ptr<SDL_GameController>(SDL_GameControllerOpen(0), SDLGameControllerDeleter());
		if (m_controller == nullptr)
		{
			LOG << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			//Get controller haptic device
			m_haptic = shared_ptr<SDL_Haptic>(SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(m_controller.get())), SDLHapticDeleter());
			if (m_haptic == NULL)
			{
				LOG << "Warning: Controller does not support haptics! SDL Error: " << SDL_GetError() << std::endl;
			}
			else
			{
				//Get initialize rumble
				if (SDL_HapticRumbleInit(m_haptic.get()) < 0)
				{
					LOG << "Warning: Unable to initialize rumble! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
		}
	}

	// Init image library
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		LOG << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return false;
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		LOG << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
		return false;
	}

	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG << "SDL_mixer could not initialize! SDL_mixer Error: " << IMG_GetError() << std::endl;
		return false;
	}

	return true;
}


void SDLApp::close()
{
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}