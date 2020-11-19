#include "SDL3DRenderer.h"
#include "../../components/SpriteSDL.h"
#include "../../components/Transform.h"
#include "../../components/Score.h"
#include "../../components/Lives.h"
#include "../../components/Clock.h"
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <iostream>
#include "../../components/Engine.h"


SDL3DRenderer::SDL3DRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDL3DRenderer::onStart(ECSManager& manager)
{
	m_sdlApp.setClearColorGL();
	m_loadedObj = m_sdlApp.loadObjFileGL("assets/models/monkey.obj");
	m_sdlApp.bufferObjDataGL(m_loadedObj);
}

void SDL3DRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	if (!m_sdlApp.isOpenGL())
	{
		return;
	}
	auto ship = manager.getAllEntitiesWithComponentType<Engine>();
	auto transform = manager.getComponentOfType<Transform>(*(ship.begin()));

	m_sdlApp.clearGL();
	m_sdlApp.renderObjGL(m_loadedObj, { transform->position.x - 400, -transform->position.y + 300, 0 }, transform->rotation - 90, { 0, 0, 1.0f }, { 13, 13, 13 });
	m_sdlApp.presentGL();
}
