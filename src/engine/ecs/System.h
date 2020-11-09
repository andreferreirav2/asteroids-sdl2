#pragma once
#include <memory>

class ECSManager;
class Inputs;

class System
{
public:
	virtual void onStart(ECSManager& manager) = 0;
	virtual void onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs, float dt) = 0;
};