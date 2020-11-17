#include "SDL3DRenderer.h"
#include "../../components/SpriteSDL.h"
#include "../../components/Transform.h"
#include "../../components/Score.h"
#include "../../components/Lives.h"
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iostream>


SDL3DRenderer::SDL3DRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDL3DRenderer::onStart(ECSManager& manager)
{
	m_sdlApp.setClearColorGL();
	m_sdlApp.setBuffersData();
}

void SDL3DRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	if (!m_sdlApp.isOpenGL())
	{
		return;
	}

	m_sdlApp.renderGL();
	m_sdlApp.presentGL();
}
