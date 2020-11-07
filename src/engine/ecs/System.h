#pragma once
#include <memory>
class ECSManager;

class System
{
public:
	virtual void onStart(ECSManager& manager) = 0;
	virtual void onUpdate(ECSManager& manager, float dt) = 0;
};