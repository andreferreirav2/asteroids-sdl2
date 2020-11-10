#pragma once
#include "../../engine/ecs/ECSManager.h"

class ShipGameController : public System
{
public:
	// Inherited via System
	virtual void onStart(ECSManager& manager) override;
	virtual void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt) override;
};

