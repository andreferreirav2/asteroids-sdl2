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
#include "systems/PhysicsDynamics.h"
#include "systems/SDLRenderer.h"
#include "systems/ShipKeyboardController.h"
#include "systems/ShipGameController.h"
#include "systems/SoundFxPlayer.h"
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
	app.loadTexture(string("assets/sprites/ships.png"));
	app.loadTexture(string("assets/sprites/sky.jpeg"));
	app.getMusic(string("assets/audio/shoot.wav"));

	ECSManager manager;

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>());
	manager.addComponent(ship, make_shared<RigidBody>(1.0f, 0.07f));
	manager.addComponent(ship, make_shared<SpriteSDL>(string("assets/sprites/ships.png"), -90.0f, false, false, uint2({ 24, 32 }), rect({ 20, 368, 396, 510 })));
	manager.addComponent(ship, make_shared<Engine>(500.0f, 150.0f));
	manager.addComponent(ship, make_shared<ShipManualControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE));
	manager.addComponent(ship, make_shared<Boundless>());
	manager.addComponent(ship, make_shared<Weapon>(0.3f));
	manager.addComponent(ship, make_shared<SoundFXSDL>(string("assets/audio/shoot.wav")));

	ShipKeyboardController shipKeyboardController = ShipKeyboardController();
	ShipGameController shipGameController = ShipGameController();
	EnginesThrusters enginesThrusters = EnginesThrusters();
	PhysicsDynamics physicsDynamics = PhysicsDynamics();
	BoundariesFlipper boundariesFlipper = BoundariesFlipper({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SDLRenderer sdlRenderer = SDLRenderer(app);
	SoundFxPlayer soundFxPlayer = SoundFxPlayer(app);

	shipKeyboardController.onStart(manager);
	shipGameController.onStart(manager);
	enginesThrusters.onStart(manager);
	physicsDynamics.onStart(manager);
	boundariesFlipper.onStart(manager);
	sdlRenderer.onStart(manager);
	soundFxPlayer.onStart(manager);

	Uint32 const startTime = SDL_GetTicks();
	Uint32 lastTick = startTime;
	while (true)
	{
		// TODO: Move to GameTimer class?
		Uint32 currentTicks = SDL_GetTicks();
		auto dt = static_cast<float>(currentTicks - lastTick) / 1000.0f;
		lastTick = currentTicks;

		auto inputs = app.parseInputs(); // parse inputs from SDL
		if (inputs->isPressed(Key::QUIT))
		{
			break;
		}

		shipKeyboardController.onUpdate(manager, inputs, dt); // pass inputs to engine / weapons
		shipGameController.onUpdate(manager, inputs, dt); // pass inputs to engine / weapons from controller
		enginesThrusters.onUpdate(manager, inputs, dt); // move all engines
		physicsDynamics.onUpdate(manager, inputs, dt); // apply velocity to position
		boundariesFlipper.onUpdate(manager, inputs, dt); // apply boundaries or 
		soundFxPlayer.onUpdate(manager, inputs, dt);
		sdlRenderer.onUpdate(manager, inputs, dt);

		Sleep(1);
	}
	return 0;
}
