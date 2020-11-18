#pragma once
#include "../input/Inputs.h"
#include "../Types.h"
#include "LoadedTexture.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <memory>
#include <string>
#include <set>
#include <map>

class SDLApp
{
public:
	// SDL general
	SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight, bool opengl = false, std::string vertexShaderPath = "", std::string fragmentShaderPath = "");
	bool init();
	void close();
	unsigned int getScreenWidth() const { return m_screenWidth; }
	unsigned int getScreenHeigth() const { return m_screenHeight; }

	// Inputs
	std::shared_ptr<Inputs> parseInputs();

	// Haptics
	void hapticRumble(float strength, unsigned int duration);

	// Music / SoundFX
	std::shared_ptr<Mix_Music> getMusic(std::string const& path);
	std::shared_ptr<Mix_Chunk> getSoundFX(std::string const& path);
	void playMusic(std::shared_ptr<Mix_Music> music);
	void playSoundFX(std::shared_ptr<Mix_Chunk> soundFX);

	// 2D Rendering
	std::shared_ptr<LoadedTexture> loadTexture(std::string const& imagePath, bool transparent = false, colorR8G8B8 const& color = {});
	void loadFont(std::string const& fontPath, int fontSize);
	std::shared_ptr<LoadedTexture> loadText(std::string const& text, colorR8G8B8 const& color = { 0xff, 0xff, 0xff });
	std::shared_ptr<LoadedTexture> getTexture(std::string const& imagePath);
	void setColor(colorR8G8B8A8 color);
	void clear(colorR8G8B8A8 const& color);
	void drawTextureFullscreen(std::shared_ptr<SDL_Texture> const texture);
	void drawTexture(std::shared_ptr<SDL_Texture> const texture, rect const& clip, rect const& coord, float angle = 0.0f, SDL_RendererFlip flipType = SDL_FLIP_NONE);
	void drawDot(uint2 const& pos, colorR8G8B8A8 const& color);
	void drawLine(uint2 const& start, uint2 const& end, colorR8G8B8A8 const& color);
	void drawRect(rect const& coord, colorR8G8B8A8 const& color, bool const& filled);
	void present();

	// 3D Rendering / OpenGL
	bool initGL();
	bool isOpenGL();
	void setOpenGL(bool openGL);
	void setClearColorGL(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);
	void setBuffersData();
	void renderGL(float x, float y, float rotate);
	void presentGL();

private:
	unsigned int m_screenWidth;
	unsigned int m_screenHeight;

	//Graphics program
	bool m_opengl;
	std::string m_vertexShaderPath;
	std::string m_fragmentShaderPath;
	GLuint m_glProgramID = 0;
	GLint m_glVertexPos3DLocation = -1;
	GLint m_glMatrix = -1;
	GLuint m_glVBO = 0;
	GLuint m_glIBO = 0;

	std::shared_ptr<SDL_Window> m_window = nullptr;
	SDL_GLContext m_glContext = nullptr;
	std::shared_ptr<SDL_Renderer> m_renderer = nullptr;
	std::shared_ptr<SDL_GameController> m_controller = nullptr;
	std::shared_ptr<SDL_Haptic> m_haptic = nullptr;
	std::shared_ptr<TTF_Font> m_font = nullptr;
	std::shared_ptr<Inputs> m_inputs = std::make_shared<Inputs>();
	std::map<std::string, std::shared_ptr<LoadedTexture>> m_textureCache = {};
	std::map<std::string, std::shared_ptr<Mix_Chunk>> m_soundFXCache = {};
	std::map<std::string, std::shared_ptr<Mix_Music>> m_musicCache = {};

};
