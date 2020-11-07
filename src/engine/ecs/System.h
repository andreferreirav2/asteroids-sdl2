#pragma once
#include <memory>
class ECSManager;

class System
{
public:
	virtual void onStart(std::shared_ptr<ECSManager> manager) = 0;
	virtual void onUpdate(std::shared_ptr<ECSManager> manager) = 0;
};