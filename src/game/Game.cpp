#include "../engine/sdl/SDLApp.h"
#include "../engine/ecs/ECSManager.h"
#include "../engine/input/Inputs.h"
#include "components/Transform.h"
#include "components/RigidBody.h"
#include "components/Engine.h"
#include "components/ShipKeyboardControls.h"
#include "systems/PhysicsDynamics.h"
#include "systems/SDLRenderer.h"
#include "systems/ShipKeyboardController.h"
#include "systems/EnginesThrusters.h"
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <assert.h>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


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
	cout << "(" << manager.getComponentOfType<Transform>(ship)->position.x << "," << manager.getComponentOfType<Transform>(ship)->position.y << ")" << endl;
	physics.onUpdate(manager, nullptr, 1.0f);
	cout << "(" << manager.getComponentOfType<Transform>(ship)->position.x << "," << manager.getComponentOfType<Transform>(ship)->position.y << ")" << endl;

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

	SDLApp app = SDLApp(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!app.init())
	{
		printf("Error starting SDLApp!");
		return 1;
	}


	ECSManager manager;

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>());
	manager.addComponent(ship, make_shared<RigidBody>(1.0f, 0.2f));
	manager.addComponent(ship, make_shared<Engine>(2.0f, 2.0f));
	manager.addComponent(ship, make_shared<ShipKeyboardControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE));

	SDLRenderer sdlRenderer = SDLRenderer(app);
	ShipKeyboardController shipKeyboardController = ShipKeyboardController();
	EnginesThrusters enginesThrusters = EnginesThrusters();
	PhysicsDynamics physicsDynamics = PhysicsDynamics();

	while (true)
	{
		auto inputs = app.parseInputs(); // parse inputs from SDL
		if (inputs->isPressed(Key::QUIT))
		{
			break;
		}
		shipKeyboardController.onUpdate(manager, inputs, 0.01f); // pass inputs to engine / weapons
		enginesThrusters.onUpdate(manager, inputs, 0.01f); // move all engines
		physicsDynamics.onUpdate(manager, inputs, 0.01f); // apply velocity to position
		sdlRenderer.onUpdate(manager, inputs, 0.01f);

		cout <<
			"(" << manager.getComponentOfType<Transform>(ship)->position.x << ", " << manager.getComponentOfType<Transform>(ship)->position.y << ")  " <<
			"(" << manager.getComponentOfType<RigidBody>(ship)->velocity.x << ", " << manager.getComponentOfType<RigidBody>(ship)->velocity.y << ")  " <<
			manager.getComponentOfType<Transform>(ship)->rotation << "rad" << endl;
	}
	return 0;
}