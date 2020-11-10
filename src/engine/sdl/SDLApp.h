#pragma once
#include "../input/Inputs.h"
#include "../Types.h"
#include "LoadedTexture.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <set>
#include <map>

class SDLApp
{
public:
	SDLApp();
	SDLApp(unsigned int m_screenWidth, unsigned int m_screenHeight);
	bool init();
	unsigned int getScreenWidth() const { return m_screenWidth; }
	unsigned int getScreenHeigth() const { return m_screenHeight; }

	std::shared_ptr<Inputs> parseInputs();
	std::shared_ptr<LoadedTexture> loadTexture(std::string const& imagePath, bool transparent = false, colorR8G8B8 const& color = {});
	std::shared_ptr<LoadedTexture> getTexture(std::string const& imagePath);

	void hapticRumble(float strength, unsigned int duration);

	void clear(colorR8G8B8A8 const& color);
	void drawTextureFullscreen(std::shared_ptr<SDL_Texture> const& texture);
	void drawTexture(std::shared_ptr<SDL_Texture> const& texture, rect const& clip, rect const& coord, float angle = 0.0f, SDL_RendererFlip flipType = SDL_FLIP_NONE);
	void drawDot(uint2 const& pos, colorR8G8B8A8 const& color);
	void drawLine(uint2 const& start, uint2 const& end, colorR8G8B8A8 const& color);
	void drawRect(rect const& coord, colorR8G8B8A8 const& color, bool const& filled);
	void present();
	void close();

private:
	void setColor(colorR8G8B8A8 color);

	unsigned int m_screenWidth;
	unsigned int m_screenHeight;
	std::shared_ptr<SDL_Window> m_window = nullptr;
	std::shared_ptr<SDL_Renderer> m_renderer = nullptr;
	std::shared_ptr<SDL_GameController> m_controller = nullptr;
	std::shared_ptr<SDL_Haptic> m_haptic = nullptr;
	std::shared_ptr<Inputs> m_inputs = std::make_shared<Inputs>();
	std::map<std::string, std::shared_ptr<LoadedTexture>> m_textureCache = {};
};
