#include "DestroyAfterEntitiesTime.h"
#include "../../components/DestroyAfterTime.h"
#include <cmath>
#include "../../components/Clock.h"


void DestroyAfterEntitiesTime::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<DestroyAfterTime>())
	{
		auto destroyAfterTime = manager.getComponentOfType<DestroyAfterTime>(e);
		destroyAfterTime->ticks -= dt;
		if (destroyAfterTime->ticks <= 0)
		{
			manager.destroyEntity(e);
		}
	}
}
