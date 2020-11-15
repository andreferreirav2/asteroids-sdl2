#include "BoundariesChecker.h"
#include "../../components/Transform.h"
#include "../../components/Boundless.h"
#include "../../components/BoundariesKill.h"
#include "../../components/PlayArea.h"
#include <cmath>


void BoundariesChecker::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	rect playArea = manager.getAllComponentsOfType<PlayArea>().begin()->get()->area; // What if there is no play area?
	for (Entity e : manager.getAllEntitiesWithComponentType<Boundless>())
	{
		auto transform = manager.getComponentOfType<Transform>(e);
		if (transform)
		{
			int x1 = playArea.x;
			int y1 = playArea.y;
			int x2 = playArea.x + playArea.w;
			int y2 = playArea.y + playArea.h;

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
		auto transform = manager.getComponentOfType<Transform>(e);
		if (transform)
		{
			auto boundariesKill = manager.getComponentOfType<BoundariesKill>(e);
			int x1 = playArea.x;
			int y1 = playArea.y;
			int x2 = playArea.x + playArea.w;
			int y2 = playArea.y + playArea.h;

			if (transform && (
				transform->position.x < x1 - boundariesKill->margin
				|| transform->position.x >= x2 + boundariesKill->margin
				|| transform->position.y < y1 - boundariesKill->margin
				|| transform->position.y >= y2 + boundariesKill->margin))
			{
				manager.destroyEntity(e);
				break;
			}
		}
	}
}
