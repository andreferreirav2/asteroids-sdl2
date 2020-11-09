#pragma once
#include "../../engine/ecs/ECSManager.h"
#include "../../engine/Types.h"

class BoundariesFlipper : public System
{
public:
	BoundariesFlipper(rect playArea);

	// Inherited via System
	virtual void onStart(ECSManager& manager) override;
	virtual void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt) override;
private:
	rect m_playArea;
};

