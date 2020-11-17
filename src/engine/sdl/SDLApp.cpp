#include "SDLApp.h"
#include "SDLResourceDeleters.h"
#include <iostream>
#include <memory>

using std::cerr;
using std::endl;
using std::shared_ptr;


SDLApp::SDLApp() :
	m_screenWidth(640),
	m_screenHeight(480),
	m_opengl(false)
{
}

SDLApp::SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight, bool opengl) :
	m_screenWidth(m_screenWidth),
	m_screenHeight(m_screenHeight),
	m_opengl(opengl)
{
}

bool SDLApp::init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC) < 0)
	{
		cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
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
		cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
		return false;
	}

	//Create context
	m_glContext = SDL_GL_CreateContext(m_window.get());
	if (m_glContext == NULL)
	{
		cerr << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		cerr << "Error initializing GLEW: " << glewGetErrorString(glewError) << endl;
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		cerr << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << endl;
	}

	//Initialize OpenGL
	if (!initGL())
	{
		cerr << "Unable to initialize OpenGL!" << endl;
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

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
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

bool SDLApp::initGL()
{
	GLenum error = GL_NO_ERROR;

	// Create gl program
	m_glProgramID = glCreateProgram();


	// Define vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexShaderSource[] = { "#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }" };
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	// Compile vertex shader
	glCompileShader(vertexShader);
	//Check for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE)
	{
		cerr << "Unable to compile vertex shader: " << vertexShader << endl;
		//printShaderLog(vertexShader);
		return false;
	}
	// Bind vertex shader gl program
	glAttachShader(m_glProgramID, vertexShader);


	// Define fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentShaderSource[] = { "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }" };
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	//Compile fragment shader
	glCompileShader(fragmentShader);
	//Check for errors
	GLint fShaderCompiled = GL_FALSE;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
	if (fShaderCompiled != GL_TRUE)
	{
		cerr << "Unable to compile fragment shader: " << fragmentShader << endl;
		//printShaderLog(fragmentShader);
		return false;
	}
	// Bind fragment shader gl program
	glAttachShader(m_glProgramID, fragmentShader);


	// Link gl program
	glLinkProgram(m_glProgramID);
	//Check for errors
	GLint programSuccess = GL_TRUE;
	glGetProgramiv(m_glProgramID, GL_LINK_STATUS, &programSuccess);
	if (programSuccess != GL_TRUE)
	{
		cerr << "Error linking program: " << m_glProgramID << endl;
		//printShaderLog(fragmentShader);
		return false;
	}


	//Get vertex attribute location
	m_glVertexPos2DLocation = glGetAttribLocation(m_glProgramID, "LVertexPos2D");
	if (m_glVertexPos2DLocation == -1)
	{
		cerr << "LVertexPos2D is not a valid glsl program variable!" << endl;
		return false;
	}
	/*
	//Initialize Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//Check for errors
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		cerr << "Error initializing OpenGL: " << gluErrorString(error) << endl;
		return false;
	}


	//Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//Check for errors
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		cerr << "Error initializing OpenGL: " << gluErrorString(error) << endl;
		return false;
	}
	*/

	return true;
}

void SDLApp::close()
{
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool SDLApp::isOpenGL()
{
	return m_opengl;
}

void SDLApp::setOpenGL(bool opengl)
{
	m_opengl = opengl;
}
