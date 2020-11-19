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
#include "../../components/Mesh.h"


SDL3DRenderer::SDL3DRenderer(SDLApp& sdlApp) :
	m_sdlApp(sdlApp)
{
}

void SDL3DRenderer::onStart(ECSManager& manager)
{
	m_sdlApp.setClearColorGL();
	for (auto& mesh : manager.getAllComponentsOfType<Mesh>())
	{
		m_sdlApp.loadObjFileGL(mesh->path);
	}
}

void SDL3DRenderer::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	if (!m_sdlApp.isOpenGL())
	{
		return;
	}

	m_sdlApp.clearGL();
	for (auto& e : manager.getAllEntitiesWithComponentType<Mesh, Transform>())
	{
		auto mesh = manager.getComponentOfType<Mesh>(e);
		auto transform = manager.getComponentOfType<Transform>(e);
		auto obj = m_sdlApp.loadObjFileGL(mesh->path);
		m_sdlApp.renderObjGL(obj, { transform->position.x - 400, -transform->position.y + 300, 0 }, transform->rotation + mesh->rotation, { 0, 0, 1.0f }, { mesh->scale, mesh->scale, mesh->scale });

	}
	m_sdlApp.presentGL();
}
