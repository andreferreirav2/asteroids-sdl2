#include "../engine/sdl/SDLApp.h"
#include "../engine/ecs/ECSManager.h"
#include "../engine/input/Inputs.h"
#include "../engine/Types.h"
#include "components/Transform.h"
#include "components/RigidBody.h"
#include "components/Engine.h"
#include "components/SpriteSDL.h"
#include "components/SoundFXSDL.h"
#include "components/ShipManualControls.h"
#include "components/Boundless.h"
#include "components/Weapon.h"
#include "components/Clock.h"
#include "systems/PhysicsDynamics.h"
#include "systems/SDLRenderer.h"
#include "systems/ShipKeyboardController.h"
#include "systems/ShipGameController.h"
#include "systems/SoundFxPlayer.h"
#include "systems/EnginesThrusters.h"
#include "systems/BoundariesFlipper.h"
#include "systems/TimePassing.h"
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

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


void test_manager()
{
	ECSManager manager;


	Entity game = manager.createEntity();
	manager.addComponent(game, make_shared<Clock>());

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
	physics.onUpdate(manager, nullptr);

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
	app.loadTexture(string("assets/sprites/ships.png"));
	app.loadTexture(string("assets/sprites/sky.jpeg"));
	app.getMusic(string("assets/audio/shoot.wav"));

	ECSManager manager;

	Entity game = manager.createEntity();
	manager.addComponent(game, make_shared<Clock>(1.0f, 60));

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>());
	manager.addComponent(ship, make_shared<RigidBody>(1.0f, 2.0f));
	manager.addComponent(ship, make_shared<SpriteSDL>(string("assets/sprites/ships.png"), -90.0f, false, false, uint2({ 16, 22 }), rect({ 20, 368, 396, 510 })));
	manager.addComponent(ship, make_shared<Engine>(300.0f, 150.f));
	manager.addComponent(ship, make_shared<ShipManualControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE));
	manager.addComponent(ship, make_shared<Boundless>());
	manager.addComponent(ship, make_shared<Weapon>(0.3f));
	manager.addComponent(ship, make_shared<SoundFXSDL>(string("assets/audio/shoot.wav")));

	TimePassing timePassing = TimePassing();
	ShipKeyboardController shipKeyboardController = ShipKeyboardController();
	ShipGameController shipGameController = ShipGameController();
	EnginesThrusters enginesThrusters = EnginesThrusters();
	PhysicsDynamics physicsDynamics = PhysicsDynamics();
	BoundariesFlipper boundariesFlipper = BoundariesFlipper({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SDLRenderer sdlRenderer = SDLRenderer(app);
	SoundFxPlayer soundFxPlayer = SoundFxPlayer(app);

	timePassing.onStart(manager);
	shipKeyboardController.onStart(manager);
	shipGameController.onStart(manager);
	enginesThrusters.onStart(manager);
	physicsDynamics.onStart(manager);
	boundariesFlipper.onStart(manager);
	sdlRenderer.onStart(manager);
	soundFxPlayer.onStart(manager);

	while (true)
	{
		auto inputs = app.parseInputs(); // parse inputs from SDL
		if (inputs->isPressed(Key::QUIT))
		{
			break;
		}

		timePassing.onUpdate(manager, inputs); // calculate passage of time
		shipKeyboardController.onUpdate(manager, inputs); // pass inputs to engine / weapons
		shipGameController.onUpdate(manager, inputs); // pass inputs to engine / weapons from controller
		enginesThrusters.onUpdate(manager, inputs); // move all engines
		physicsDynamics.onUpdate(manager, inputs); // apply velocity to position
		boundariesFlipper.onUpdate(manager, inputs); // apply boundaries or 
		sdlRenderer.onUpdate(manager, inputs);
		soundFxPlayer.onUpdate(manager, inputs);
	}
	return 0;
}
