#include "../engine/sdl/SDLApp.h"
#include "../engine/ecs/ECSManager.h"
#include "../engine/input/Inputs.h"
#include "../engine/Types.h"
#include "components/Transform.h"
#include "components/RigidBody.h"
#include "components/Engine.h"
#include "components/SpriteSDL.h"
#include "components/ShipKeyboardControls.h"
#include "components/Boundless.h"
#include "systems/PhysicsDynamics.h"
#include "systems/SDLRenderer.h"
#include "systems/ShipKeyboardController.h"
#include "systems/EnginesThrusters.h"
#include "systems/BoundariesFlipper.h"
#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

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

	// Pre load assets
	app.loadTexture(string("assets/sprites/ship.bmp"), true, colorR8G8B8({ 0xff, 0xff, 0xff }));

	ECSManager manager;

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>());
	manager.addComponent(ship, make_shared<RigidBody>(1.0f, 0.1f));
	manager.addComponent(ship, make_shared<SpriteSDL>(string("assets/sprites/ship.bmp"), uint2({ 24, 24 }), rect({ 80, 80, 80, 80 })));
	manager.addComponent(ship, make_shared<Engine>(20.0f, 0.1f));
	manager.addComponent(ship, make_shared<ShipKeyboardControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE));
	manager.addComponent(ship, make_shared<Boundless>());

	Entity ship2 = manager.createEntity();
	manager.addComponent(ship2, make_shared<Transform>(200.0f, 200.0f));
	manager.addComponent(ship2, make_shared<RigidBody>(1.0f, 0.1f));
	manager.addComponent(ship2, make_shared<SpriteSDL>(string("assets/sprites/ship.bmp"), uint2({ 24, 24 }), rect({ 0, 80, 80, 80 })));
	manager.addComponent(ship2, make_shared<Engine>(20.0f, 0.1f));
	manager.addComponent(ship2, make_shared<ShipKeyboardControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE));
	manager.addComponent(ship2, make_shared<Boundless>());

	ShipKeyboardController shipKeyboardController = ShipKeyboardController();
	EnginesThrusters enginesThrusters = EnginesThrusters();
	PhysicsDynamics physicsDynamics = PhysicsDynamics();
	BoundariesFlipper boundariesFlipper = BoundariesFlipper({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SDLRenderer sdlRenderer = SDLRenderer(app);

	shipKeyboardController.onStart(manager);
	enginesThrusters.onStart(manager);
	physicsDynamics.onStart(manager);
	boundariesFlipper.onStart(manager);
	sdlRenderer.onStart(manager);

	while (true)
	{
		auto dt = 0.1f; // TODO: calculate dt

		auto inputs = app.parseInputs(); // parse inputs from SDL
		if (inputs->isPressed(Key::QUIT))
		{
			break;
		}

		if (inputs->isPressed(Key::KEY_UP) && inputs->isPressed(Key::KEY_RIGHT))
		{
			break;
		}

		shipKeyboardController.onUpdate(manager, inputs, dt); // pass inputs to engine / weapons
		enginesThrusters.onUpdate(manager, inputs, dt); // move all engines
		physicsDynamics.onUpdate(manager, inputs, dt); // apply velocity to position
		boundariesFlipper.onUpdate(manager, inputs, dt); // apply boundaries or 
		sdlRenderer.onUpdate(manager, inputs, dt);


		cout <<
			"(" << manager.getComponentOfType<Transform>(ship)->position.x << ", " << manager.getComponentOfType<Transform>(ship)->position.y << ")  " <<
			"(" << manager.getComponentOfType<RigidBody>(ship)->velocity.x << ", " << manager.getComponentOfType<RigidBody>(ship)->velocity.y << ")  " <<
			manager.getComponentOfType<Transform>(ship)->rotation << "rad" << endl;

		Sleep(1);
	}
	return 0;
}