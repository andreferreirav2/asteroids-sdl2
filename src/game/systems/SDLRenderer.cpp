#include "SDLRenderer.h"

SDLRenderer::SDLRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDLRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
{
	m_sdlApp.clear({ 0x00, 0x00, 0x00, 0x00 });

	auto shipTexture = m_sdlApp.loadTexture("assets/sprites/ship.bmp");
	m_sdlApp.drawTexture(shipTexture);

	m_sdlApp.drawRect(
		{ m_sdlApp.getScreenWidth() / 4, m_sdlApp.getScreenHeigth() / 4 },
		{ m_sdlApp.getScreenWidth() / 2, m_sdlApp.getScreenHeigth() / 2 },
		{ 0xFF, 0x00, 0x00, 0xFF },
		true);

	m_sdlApp.drawRect(
		{ m_sdlApp.getScreenWidth() / 6, m_sdlApp.getScreenHeigth() / 6 },
		{ m_sdlApp.getScreenWidth() * 2 / 3, m_sdlApp.getScreenHeigth() * 2 / 3 },
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
	m_sdlApp.present();
}
