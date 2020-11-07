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
	RigidBody(float vx, float vy) : vx(vx), vy(vy) {}
	float vx, vy;
};

void test_manager()
{
	ECSManager manager;

	assert(manager.getAllComponentsOfType(GetComponentType<Transform>()).size() == 0);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<Transform>()).size() == 0);
	assert(manager.getAllComponentsOfType(GetComponentType<RigidBody>()).size() == 0);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<RigidBody>()).size() == 0);

	cout << manager.createEntity() << endl;

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>(1.0f, 2.0f));
	manager.addComponent(ship, make_shared<RigidBody>(0.0f, 0.0f));
	Entity ship1 = manager.createEntity();
	manager.addComponent(ship1, make_shared<Transform>(1.0f, 2.0f));

	assert(manager.getAllComponentsOfType(GetComponentType<Transform>()).size() == 2);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<Transform>()).size() == 2);
	assert(manager.getAllComponentsOfType(GetComponentType<RigidBody>()).size() == 1);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<RigidBody>()).size() == 1);

	manager.removeComponentOfType(ship, GetComponentType<Transform>());

	assert(manager.getAllComponentsOfType(GetComponentType<Transform>()).size() == 1);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<Transform>()).size() == 1);
	assert(manager.getAllComponentsOfType(GetComponentType<RigidBody>()).size() == 1);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<RigidBody>()).size() == 1);

	manager.destroyEntity(ship);

	assert(manager.getAllComponentsOfType(GetComponentType<Transform>()).size() == 1);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<Transform>()).size() == 1);
	assert(manager.getAllComponentsOfType(GetComponentType<RigidBody>()).size() == 0);
	assert(manager.getAllEntitiesWithComponentType(GetComponentType<RigidBody>()).size() == 0);
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