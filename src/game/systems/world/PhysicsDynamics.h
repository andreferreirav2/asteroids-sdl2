#pragma once
#include "../../../engine/ecs/ECSManager.h"

class PhysicsDynamics : public System
{
public:
	// Inherited via System
	virtual void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs) override;
};

