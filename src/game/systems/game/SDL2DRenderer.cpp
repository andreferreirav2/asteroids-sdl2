#include "SDL2DRenderer.h"
#include "../../components/SpriteSDL.h"
#include "../../components/Transform.h"
#include "../../components/Score.h"
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include "../../components/Lives.h"

std::string toString(int value, int zeros)
{
	std::stringstream ss;
	ss << std::setw(zeros) << std::setfill(' ') << value;
	return ss.str();
}

SDL2DRenderer::SDL2DRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDL2DRenderer::setTexture(std::shared_ptr<SpriteSDL> sprite)
{
	sprite->loadedTexture = m_sdlApp.getTexture(sprite->path); // Reuse same texture for all sprites, by path
	if (sprite->size.x == 0 && sprite->size.y == 0)
	{
		sprite->size = sprite->loadedTexture->dimentions;
	}
}

void SDL2DRenderer::renderHUD(ECSManager& manager)
{
	// Score
	int player = 0;
	std::set<int> currentScores;
	for (auto e : manager.getAllEntitiesWithComponentType<Score>())
	{
		auto scoreBoard = manager.getComponentOfType<Score>(e);
		int score = scoreBoard->score;

		currentScores.insert(score);
		std::shared_ptr<LoadedTexture> text;
		if (m_scoresTextTextureCache.find(score) != m_scoresTextTextureCache.end())
		{
			text = m_scoresTextTextureCache[score];
		}
		else
		{
			text = m_sdlApp.loadText(toString(score, 8));
			m_scoresTextTextureCache[score] = text;
		}


		int w = text->dimentions.x;
		int h = text->dimentions.y;
		int x, y;
		switch (player++)
		{
		case 0:
			x = 20 + 128 - w;
			y = 20;
			break;
		case 1:
			x = m_sdlApp.getScreenWidth() - 20 - w;
			y = 20;
			break;
		case 2:
			x = 20 + 128 - w;
			y = m_sdlApp.getScreenHeigth() - 52 - h;
			break;
		case 3:
			x = m_sdlApp.getScreenWidth() - 20 - w;
			y = m_sdlApp.getScreenHeigth() - 52 - h;
			break;
		default:
			x = 0;
			y = 0;
			std::cerr << "More than 4 players found!!! " << std::endl;
			break;
		}

		m_sdlApp.drawTexture(
			text->texture,
			{ }, // TODO: should clipping be mandatory?
			{ x, y, text->dimentions.x, text->dimentions.y });


		auto sprite = manager.getComponentOfType<SpriteSDL>(e);
		auto lives = manager.getComponentOfType<Lives>(e);
		if (sprite && lives)
		{
			float xPos = static_cast<float>(x + text->dimentions.x + 4 - sprite->size.x);
			float yPos = static_cast<float>(text->dimentions.y + y + 12);
			for (int i = 0; i < lives->left; i++)
			{
				drawSpriteSDL(sprite, { xPos - (sprite->size.x + 4) * i, yPos }, 90);
			}
		}
	}

	if (m_scoresTextTextureCache.size() > currentScores.size())
	{
		for (auto it = m_scoresTextTextureCache.cbegin(); it != m_scoresTextTextureCache.cend();)
		{
			if (currentScores.find(it->first) == currentScores.end())
			{
				m_scoresTextTextureCache.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}
}

void SDL2DRenderer::onStart(ECSManager& manager)
{
	for (auto& sprite : manager.getAllComponentsOfType<SpriteSDL>())
	{
		setTexture(sprite);
	}
}


void SDL2DRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	if (m_sdlApp.isOpenGL())
	{
		return;
	}

	m_sdlApp.clear({ 0x00, 0x00, 0x00, 0xff });

	for (auto& e : manager.getAllEntitiesWithComponentType<SpriteSDL, Transform>())
	{
		auto sprite = manager.getComponentOfType<SpriteSDL>(e);
		auto transform = manager.getComponentOfType<Transform>(e);

		drawSpriteSDL(sprite, transform->position, transform->rotation, transform->scale);
	}

	renderHUD(manager);

	m_sdlApp.present();
}

void SDL2DRenderer::drawSpriteSDL(std::shared_ptr<SpriteSDL>& sprite, float2 position, float rotation, float2 scale)
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

	unsigned int sizeX = sprite->size.x * scale.x;
	unsigned int sizeY = sprite->size.y * scale.y;
	m_sdlApp.drawTexture(
		sprite->loadedTexture->texture,
		sprite->cliping,
		{ static_cast<int>(position.x - sizeX / 2), static_cast<int>(position.y - sizeY / 2), sizeX, sizeY },
		-rotation - sprite->rotationAngle,
		flipType);
}
