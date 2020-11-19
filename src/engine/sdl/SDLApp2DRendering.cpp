#include "SDLApp.h"
#include "LoadedTexture.h"
#include "SDLResourceDeleters.h"
#include <iostream>
#include <string>
#include <memory>
#include <set>

using std::cerr;
using std::endl;
using std::string;
using std::shared_ptr;
using std::make_shared;


void SDLApp::setColor(colorR8G8B8A8 color)
{
	SDL_SetRenderDrawColor(m_renderer.get(), color.r, color.g, color.b, color.a);
}

void SDLApp::clear(colorR8G8B8A8 const& color)
{
	setColor(color);
	SDL_RenderClear(m_renderer.get());
}

void SDLApp::drawTextureFullscreen(std::shared_ptr<SDL_Texture> const texture)
{
	SDL_RenderCopy(m_renderer.get(), texture.get(), NULL, NULL);
}

void SDLApp::drawTexture(std::shared_ptr<SDL_Texture> const texture, rect const& clip, rect const& coord, float angle, SDL_RendererFlip flipType)
{
	SDL_Rect* pClp = NULL;
	SDL_Rect clp = { clip.x, clip.y, static_cast<int>(clip.w), static_cast<int>(clip.h) };
	SDL_Rect dst = { coord.x, coord.y, static_cast<int>(coord.w), static_cast<int>(coord.h) };

	if (clip.w != 0 && clip.h != 0)
	{
		pClp = &clp;
	}

	SDL_RenderCopyEx(m_renderer.get(), texture.get(), pClp, &dst, angle, NULL, flipType);
}

void SDLApp::drawDot(uint2 const& pos, colorR8G8B8A8 const& color)
{
	setColor(color);
	SDL_RenderDrawPoint(m_renderer.get(), pos.x, pos.y);
}

void SDLApp::drawLine(uint2 const& start, uint2 const& end, colorR8G8B8A8 const& color)
{
	setColor(color);
	SDL_RenderDrawLine(m_renderer.get(), start.x, start.y, end.x, end.y);
}

void SDLApp::drawRect(rect const& coord, colorR8G8B8A8 const& color, bool const& filled)
{
	SDL_Rect rect = { coord.x, coord.y, static_cast<int>(coord.w), static_cast<int>(coord.h) };
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

shared_ptr<LoadedTexture> SDLApp::loadTexture(string const& imagePath, bool transparent, colorR8G8B8 const& color)
{
	auto it = m_textureCache.find(imagePath);
	if (it != m_textureCache.end() && it->second)
	{
		return it->second;
	}

	shared_ptr<SDL_Texture> texture = nullptr;
	shared_ptr<SDL_Surface> loadedSurface = shared_ptr<SDL_Surface>(IMG_Load(imagePath.c_str()), SDLSurfaceDeleter());
	if (loadedSurface == NULL)
	{
		cerr << "Unable to load image " << imagePath << "! SDL Error: " << SDL_GetError() << endl;
		return nullptr;
	}

	if (transparent)
	{
		SDL_SetColorKey(loadedSurface.get(), SDL_TRUE, SDL_MapRGB(loadedSurface->format, color.r, color.g, color.b));
	}

	texture = shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(m_renderer.get(), loadedSurface.get()), SDLTextureDeleter());
	if (texture == NULL)
	{
		cerr << "Unable to create texture from  " << imagePath << "! SDL Error: " << SDL_GetError() << endl;
		return nullptr;
	}
	auto ltexture = std::make_shared<LoadedTexture>(texture, imagePath, uint2({ static_cast<unsigned int>(loadedSurface->w), static_cast<unsigned int>(loadedSurface->h) }));
	m_textureCache.insert_or_assign(imagePath, ltexture);

	return ltexture;
}

shared_ptr<LoadedTexture> SDLApp::getTexture(string const& imagePath)
{
	auto it = m_textureCache.find(imagePath);
	if (it == m_textureCache.end()) // if not yet loaded, load it now
	{
		return loadTexture(imagePath);
	}
	return it->second;
}

void SDLApp::loadFont(std::string const& fontPath, int fontSize)
{
	m_font.reset();
	m_font = std::shared_ptr<TTF_Font>(TTF_OpenFont(fontPath.c_str(), fontSize), TTFFontDeleter());
}

std::shared_ptr<LoadedTexture> SDLApp::loadText(std::string const& text, colorR8G8B8 const& color)
{
	//Render text surface
	auto textSurface = std::shared_ptr<SDL_Surface>(TTF_RenderText_Solid(m_font.get(), text.c_str(), SDL_Color({ color.r, color.g, color.b })), SDLSurfaceDeleter());
	if (textSurface == NULL)
	{
		cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
		return nullptr;
	}

	//Create texture from surface pixels
	auto texture = std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(m_renderer.get(), textSurface.get()), SDLTextureDeleter());
	if (texture == NULL)
	{
		cerr << "Unable to create texture from rendered text! SDL Error " << SDL_GetError() << endl;
		return nullptr;
	}

	auto ltexture = std::make_shared<LoadedTexture>(texture, text, uint2({ static_cast<unsigned int>(textSurface->w), static_cast<unsigned int>(textSurface->h) }));
	//m_textureCache.insert_or_assign(text, ltexture);
	return ltexture;
}