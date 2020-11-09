#include "BoundariesFlipper.h"
#include "../components/Transform.h"
#include "../components/Boundless.h"
#include <cmath>


BoundariesFlipper::BoundariesFlipper(rect playArea) : m_playArea(playArea) {}

void BoundariesFlipper::onStart(ECSManager& manager) {}

void BoundariesFlipper::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt)
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
}
