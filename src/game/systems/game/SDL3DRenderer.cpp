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

std::string toString(int value, int zeros)
{
	std::stringstream ss;
	ss << std::setw(zeros) << std::setfill(' ') << value;
	return ss.str();
}


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
		m_sdlApp.renderObjGL(obj,
			{ transform->position.x - 400, -transform->position.y + 300, 0 },
			transform->rotation + mesh->rotation,
			{ 0, 0, 1.0f },
			{ mesh->scale, mesh->scale, mesh->scale },
			{ mesh->colorDiffuse.x, mesh->colorDiffuse.y, mesh->colorDiffuse.z },
			mesh->emissiveness);

	}

	renderHUD(manager);

	m_sdlApp.presentGL();
}


void SDL3DRenderer::renderHUD(ECSManager& manager)
{
	// Score
	int player = 0;
	std::set<int> currentScores;
	for (auto e : manager.getAllEntitiesWithComponentType<Score>())
	{
		auto scoreBoard = manager.getComponentOfType<Score>(e);
		int score = scoreBoard->score;

		int w = 130; // calculate width of text
		int h = 16; // calculate height of text
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
			y = m_sdlApp.getScreenHeigth() - 68 - h;
			break;
		case 3:
			x = m_sdlApp.getScreenWidth() - 20 - w;
			y = m_sdlApp.getScreenHeigth() - 68 - h;
			break;
		default:
			x = 0;
			y = 0;
			std::cerr << "More than 4 players found!!! " << std::endl;
			break;
		}

		auto mesh = manager.getComponentOfType<Mesh>(e);
		auto lives = manager.getComponentOfType<Lives>(e);
		if (mesh && lives)
		{
			auto mesh = manager.getComponentOfType<Mesh>(e);
			auto transform = manager.getComponentOfType<Transform>(e);
			auto obj = m_sdlApp.loadObjFileGL(mesh->path);

			float xPos = static_cast<float>(-400 + 120 + x);
			float yPos = static_cast<float>(300 - y - 48);
			for (int i = 0; i < lives->left; i++)
			{
				m_sdlApp.renderObjGL(obj,
					{ xPos - (16 * i),yPos, 0 },
					90,
					{ 0, 0, 1.0f },
					{ mesh->scale, mesh->scale, mesh->scale },
					{ mesh->colorDiffuse.x, mesh->colorDiffuse.y, mesh->colorDiffuse.z },
					mesh->emissiveness);
			}
		}

		m_sdlApp.renderRenderTextGL(toString(score, 8), x, 600 - y - 28, 0.6f, { 1,1,1 });
	}
}
