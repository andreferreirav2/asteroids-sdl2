#include "SDLApp.h"
#include "engine/ecs/ECSManager.h"
#include "engine/ecs/Component.h"
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <assert.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Transform : public Component
{
public:
	Transform(float x, float y) : x(x), y(y) {}
	float x, y;
};

class RigidBody : public Component
{
public:
	RigidBody(float mass, float vx, float vy) : mass(mass), vx(vx), vy(vy) {}
	float mass, vx, vy;
};


class PhysicsDynamics : public System
{
public:
	// Inherited via System
	void onStart(ECSManager& manager) override
	{
	}

	void onUpdate(ECSManager& manager, float dt) override
	{
		for (Entity e : manager.getAllEntitiesWithComponentType<RigidBody>())
		{
			auto rb = manager.getComponentOfType<RigidBody>(e);
			auto transform = manager.getComponentOfType<Transform>(e);

			rb->vy += -9.81f * rb->mass * dt;
			transform->y += rb->vy * dt;
		}
	}
};

void test_manager()
{
	ECSManager manager;

	assert(manager.getAllComponentsOfType<Transform>().size() == 0);
	assert(manager.getAllEntitiesWithComponentType<Transform>().size() == 0);
	assert(manager.getAllComponentsOfType<RigidBody>().size() == 0);
	assert(manager.getAllEntitiesWithComponentType<RigidBody>().size() == 0);

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>(0.0f, 0.0f));
	manager.addComponent(ship, make_shared<RigidBody>(1.0f, 0.0f, 0.0f));
	Entity ship1 = manager.createEntity();
	manager.addComponent(ship1, make_shared<Transform>(1.0f, 2.0f));


	PhysicsDynamics physics = PhysicsDynamics();
	cout << "(" << manager.getComponentOfType<Transform>(ship)->x << "," << manager.getComponentOfType<Transform>(ship)->y << ")" << endl;
	physics.onUpdate(manager, 1.0f);
	cout << "(" << manager.getComponentOfType<Transform>(ship)->x << "," << manager.getComponentOfType<Transform>(ship)->y << ")" << endl;

	assert(manager.getAllComponentsOfType<Transform>().size() == 2);
	assert(manager.getAllEntitiesWithComponentType<Transform>().size() == 2);
	assert(manager.getAllComponentsOfType<RigidBody>().size() == 1);
	assert(manager.getAllEntitiesWithComponentType<RigidBody>().size() == 1);

	manager.removeComponentOfType<Transform>(ship);

	assert(manager.getAllComponentsOfType<Transform>().size() == 1);
	assert(manager.getAllEntitiesWithComponentType<Transform>().size() == 1);
	assert(manager.getAllComponentsOfType<RigidBody>().size() == 1);
	assert(manager.getAllEntitiesWithComponentType<RigidBody>().size() == 1);

	manager.destroyEntity(ship);

	assert(manager.getAllComponentsOfType<Transform>().size() == 1);
	assert(manager.getAllEntitiesWithComponentType<Transform>().size() == 1);
	assert(manager.getAllComponentsOfType<RigidBody>().size() == 0);
	assert(manager.getAllEntitiesWithComponentType<RigidBody>().size() == 0);
}

int main(int argc, char* args[])
{
	test_manager();


	/*
	SDLApp app = SDLApp(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!app.init())
	{
		printf("Error starting SDLApp!");
		return 1;
	}
	app.run();*/

	cin.get(); // TODO: Delete. to preserve the console regardless of debug or not
	return 0;
}