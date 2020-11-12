#include "BoundariesChecker.h"
#include "../components/Transform.h"
#include "../components/Boundless.h"
#include "../components/BoundariesKill.h"
#include <cmath>


BoundariesChecker::BoundariesChecker(rect playArea) : m_playArea(playArea) {}

void BoundariesChecker::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	for (Entity e : manager.getAllEntitiesWithComponentType<Boundless>())
	{
		auto transform = manager.getComponentOfType<Transform>(e);
		if (transform)
		{
			int x1 = m_playArea.x;
			int y1 = m_playArea.y;
			int x2 = m_playArea.x + m_playArea.w;
			int y2 = m_playArea.y + m_playArea.h;

			if (transform->position.x < x1)
			{
				transform->position.x = x2 - (x1 - transform->position.x);
			}
			else if (transform->position.x >= x2)
			{
				transform->position.x = x1 + (transform->position.x - x2);
			}

			if (transform->position.y < y1)
			{
				transform->position.y = y2 - (y1 - transform->position.y);
			}
			else if (transform->position.y >= y2)
			{
				transform->position.y = y1 + (transform->position.y - y2);
			}
		}
	}

	for (Entity e : manager.getAllEntitiesWithComponentType<BoundariesKill>())
	{
		auto boundariesKill = manager.getComponentOfType<BoundariesKill>(e);
		auto transform = manager.getComponentOfType<Transform>(e);
		if (boundariesKill && transform)
		{
			int x1 = m_playArea.x;
			int y1 = m_playArea.y;
			int x2 = m_playArea.x + m_playArea.w;
			int y2 = m_playArea.y + m_playArea.h;

			if (transform->position.x < x1 - boundariesKill->margin
				|| transform->position.x >= x2 + boundariesKill->margin
				|| transform->position.y < y1 - boundariesKill->margin
				|| transform->position.y >= y2 + boundariesKill->margin)
			{
				manager.destroyEntity(e);
				break;
			}
		}
	}
}
