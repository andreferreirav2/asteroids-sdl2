#include "SDLRenderer.h"
#include "../components/SpriteSDL.h"
#include "../components/Transform.h"

SDLRenderer::SDLRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDLRenderer::setTexture(std::shared_ptr<SpriteSDL> sprite)
{
	sprite->loadedTexture = m_sdlApp.getTexture(sprite->path); // Reuse same texture for all sprites, by path
	if (sprite->size.x == 0 && sprite->size.y == 0)
	{
		sprite->size = sprite->loadedTexture->dimentions;
	}
}

void SDLRenderer::onStart(ECSManager& manager)
{
	for (auto& sprite : manager.getAllComponentsOfType<SpriteSDL>())
	{
		setTexture(sprite);
	}
}

void SDLRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	m_sdlApp.clear({ 0x00, 0x00, 0x00, 0xff });

	/*
	auto sky = m_sdlApp.getTexture("assets/sprites/sky.jpeg");
	m_sdlApp.drawTextureFullscreen(sky->texture);

	m_sdlApp.drawRect(
		{ static_cast<int>(m_sdlApp.getScreenWidth() / 4), static_cast<int>(m_sdlApp.getScreenHeigth() / 4),
		  static_cast<unsigned int>(m_sdlApp.getScreenWidth() / 2), static_cast<unsigned int>(m_sdlApp.getScreenHeigth() / 2) },
		{ 0xFF, 0x00, 0x00, 0xFF },
		true);

	m_sdlApp.drawRect(
		{ static_cast<int>(m_sdlApp.getScreenWidth() / 6), static_cast<int>(m_sdlApp.getScreenHeigth() / 6),
		  static_cast<unsigned int>(m_sdlApp.getScreenWidth() * 2 / 3), static_cast<unsigned int>(m_sdlApp.getScreenHeigth() * 2 / 3) },
		{ 0x00, 0xFF, 0x00, 0xFF },
		false);

	m_sdlApp.drawLine(
		{ 0, m_sdlApp.getScreenHeigth() },
		{ m_sdlApp.getScreenWidth(), 0 },
		{ 0x00, 0x00, 0xFF, 0xFF });

	for (unsigned int i = 0; i < m_sdlApp.getScreenHeigth(); i += 4)
	{
		m_sdlApp.drawDot(
			{ m_sdlApp.getScreenWidth() / 2, i },
			{ 0xFF, 0xFF, 0x00, 0xFF });
	}
	*/

	for (auto& e : manager.getAllEntitiesWithComponentType<Transform>())
	{
		auto sprite = manager.getComponentOfType<SpriteSDL>(e);
		auto transform = manager.getComponentOfType<Transform>(e);
		if (sprite)
		{
			SDL_RendererFlip flipType;
			if (sprite->flipVertical)
			{
				flipType = SDL_FLIP_VERTICAL;
			}
			else if (sprite->flipHorizontal)
			{
				flipType = SDL_FLIP_HORIZONTAL;
			}
			else
			{
				flipType = SDL_FLIP_NONE;
			}

			if (sprite->loadedTexture == nullptr) // If sprite is new and texture is not loaded yet
			{
				setTexture(sprite);
			}

			unsigned int sizeX = sprite->size.x * transform->scale.x;
			unsigned int sizeY = sprite->size.y * transform->scale.y;
			m_sdlApp.drawTexture(
				sprite->loadedTexture->texture,
				sprite->cliping,
				{ static_cast<int>(transform->position.x - sizeX / 2), static_cast<int>(transform->position.y - sizeY / 2),
				  sizeX, sizeY },
				-transform->rotation - sprite->rotationAngle,
				flipType);
		}
		else
		{
			m_sdlApp.drawRect(
				{ static_cast<int>(transform->position.x) - 12, static_cast<int>(transform->position.y) - 12,
				  24, 24 },
				{ 0x00, 0x00, 0x00, 0xFF },
				true);
		}
	}

	m_sdlApp.present();
}
