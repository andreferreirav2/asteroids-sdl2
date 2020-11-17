#include "SDL3DRenderer.h"
#include "../../components/SpriteSDL.h"
#include "../../components/Transform.h"
#include "../../components/Score.h"
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include "../../components/Lives.h"


SDL3DRenderer::SDL3DRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDL3DRenderer::onStart(ECSManager& manager)
{
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
