#include "SDLRenderer.h"

SDLRenderer::SDLRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDLRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	m_sdlApp.fillScreen(0xff, 0xff, 0xff);

	auto bmpPic = m_sdlApp.loadBMP("assets/sprites/ship.bmp");
	m_sdlApp.drawSurface(bmpPic);

	m_sdlApp.flush();
}
