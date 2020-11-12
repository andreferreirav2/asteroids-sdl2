#pragma once
#include "../../engine/ecs/ECSManager.h"
#include "../../engine/Types.h"

class BoundariesChecker : public System
{
public:
	BoundariesChecker(rect playArea);

	// Inherited via System
	virtual void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs) override;
private:
	rect m_playArea;
};

