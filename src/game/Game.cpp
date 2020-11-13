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
#include "components/BoundariesKill.h"
#include "components/Weapon.h"
#include "components/Clock.h"
#include "components/CircleCollider.h"
#include "systems/PhysicsDynamics.h"
#include "systems/SDLRenderer.h"
#include "systems/ShipKeyboardController.h"
#include "systems/ShipGameController.h"
#include "systems/SoundFxPlayer.h"
#include "systems/EnginesThrusters.h"
#include "systems/BoundariesChecker.h"
#include "systems/TimePassing.h"
#include "systems/PhysicsCollisions.h"
#include "systems/WeaponFiring.h"
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

#define RAD_2_DEG 57.2958f
#define DEG_2_RAG 0.0174533f

int PLAYER_COLLIDER_LAYER = 1 << 0;
int PLAYER_WEAPON_COLLIDER_LAYER = 1 << 1;
int ENEMY_COLLIDER_LAYER = 1 << 2;
int ENEMY_WEAPON_COLLIDER_LAYER = 1 << 3;
int ASTEROIDS_COLLIDER_LAYER = 1 << 4;

int PLAYER_COLLIDES_WITH = ENEMY_COLLIDER_LAYER | ENEMY_WEAPON_COLLIDER_LAYER | ASTEROIDS_COLLIDER_LAYER;
int PLAYER_WEAPON_COLLIDES_WITH = ENEMY_COLLIDER_LAYER | ENEMY_WEAPON_COLLIDER_LAYER | ASTEROIDS_COLLIDER_LAYER;
int ENEMY_COLLIDES_WITH = PLAYER_COLLIDER_LAYER | PLAYER_WEAPON_COLLIDER_LAYER;
int ENEMY_WEAPON_COLLIDES_WITH = PLAYER_COLLIDER_LAYER | PLAYER_WEAPON_COLLIDER_LAYER | ASTEROIDS_COLLIDER_LAYER;
int ASTEROIDS_COLLIDES_WITH = PLAYER_COLLIDER_LAYER | PLAYER_WEAPON_COLLIDER_LAYER | ENEMY_WEAPON_COLLIDER_LAYER;


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


	assert(manager.getComponentOfType<RigidBody>(ship1) == nullptr);


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
	app.loadTexture(string("assets/sprites/atlas.png"));
	app.getMusic(string("assets/audio/shoot.wav"));

	auto shipSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 16, 24 }), rect({ 0, 0, 64, 96 }));
	auto shotSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 2, 3 }), rect({ 0, 160, 32, 48 }));
	auto asteroidSmallSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 16, 16 }), rect({ 0, 96, 64, 64 }));
	auto asteroidMediumSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 40, 40 }), rect({ 64, 0, 160, 160 }));
	auto asteroidLargeSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 64, 64 }), rect({ 224, 0, 288, 288 }));

	ECSManager manager;

	Entity game = manager.createEntity();
	manager.addComponent(game, make_shared<Clock>(1.0f, 1000));

	Entity ship = manager.createEntity();
	manager.addComponent(ship, make_shared<Transform>(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 90.f));
	manager.addComponent(ship, make_shared<RigidBody>(1.0f, 2.0f));
	manager.addComponent(ship, shipSprite);
	manager.addComponent(ship, make_shared<Engine>(300.0f, 150.f));
	manager.addComponent(ship, make_shared<ShipManualControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE));
	manager.addComponent(ship, make_shared<Boundless>());
	manager.addComponent(ship, make_shared<SoundFXSDL>(string("assets/audio/shoot.wav")));
	manager.addComponent(ship, make_shared<CircleCollider>(7.0f, PLAYER_COLLIDER_LAYER, PLAYER_COLLIDES_WITH));
	manager.addComponent(ship, make_shared<Weapon>(0.3f, [&](shared_ptr<Transform> gun)
		{
			Entity shot = manager.createEntity();
			auto shotRb = std::make_shared<RigidBody>(1.0f, 0.0f);
			manager.addComponent(shot, std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation));
			manager.addComponent(shot, std::make_shared<RigidBody>(1.0f, 0.0f, 200 * cos(gun->rotation * DEG_2_RAG), -200 * sin(gun->rotation * DEG_2_RAG)));
			manager.addComponent(shot, shotSprite);
			manager.addComponent(shot, std::make_shared<CircleCollider>(4.0f, PLAYER_WEAPON_COLLIDER_LAYER, PLAYER_WEAPON_COLLIDES_WITH));
			manager.addComponent(shot, make_shared<BoundariesKill>());
		}));

	// small ast
	for (int i = 0; i < 20; i++)
	{
		Entity ast = manager.createEntity();
		manager.addComponent(ast, make_shared<Transform>(0, 100 * i, i * 30.0f));
		manager.addComponent(ast, make_shared<RigidBody>(1.0f, 0.0f, 10.0f, 0.0f));
		manager.addComponent(ast, asteroidSmallSprite);
		manager.addComponent(ast, make_shared<CircleCollider>(8.0f, ASTEROIDS_COLLIDER_LAYER, ASTEROIDS_COLLIDES_WITH));
	}

	// medium ast
	for (int i = 0; i < 20; i++)
	{
		Entity ast = manager.createEntity();
		manager.addComponent(ast, make_shared<Transform>(0, 100 * i, i * 20.0f));
		manager.addComponent(ast, make_shared<RigidBody>(1.0f, 0.0f, 10.0f, 0.0f));
		manager.addComponent(ast, asteroidMediumSprite);
		manager.addComponent(ast, make_shared<CircleCollider>(20.0f, ASTEROIDS_COLLIDER_LAYER, ASTEROIDS_COLLIDES_WITH));
	}

	// big ast
	for (int i = 0; i < 20; i++)
	{
		Entity ast = manager.createEntity();
		manager.addComponent(ast, make_shared<Transform>(0, 100 * i, i * 10.0f));
		manager.addComponent(ast, make_shared<RigidBody>(1.0f, 0.0f, 10.0f, 0.0f));
		manager.addComponent(ast, asteroidLargeSprite);
		manager.addComponent(ast, make_shared<CircleCollider>(32.0f, ASTEROIDS_COLLIDER_LAYER, ASTEROIDS_COLLIDES_WITH));
	}

	TimePassing timePassing = TimePassing();
	ShipKeyboardController shipKeyboardController = ShipKeyboardController();
	ShipGameController shipGameController = ShipGameController();
	EnginesThrusters enginesThrusters = EnginesThrusters();
	WeaponFiring weaponFiring = WeaponFiring();
	PhysicsDynamics physicsDynamics = PhysicsDynamics();
	PhysicsCollisions physicsCollisions = PhysicsCollisions();
	BoundariesChecker boundariesChecker = BoundariesChecker({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SDLRenderer sdlRenderer = SDLRenderer(app);
	SoundFxPlayer soundFxPlayer = SoundFxPlayer(app);

	timePassing.onStart(manager);
	shipKeyboardController.onStart(manager);
	shipGameController.onStart(manager);
	enginesThrusters.onStart(manager);
	weaponFiring.onStart(manager);
	physicsDynamics.onStart(manager);
	boundariesChecker.onStart(manager);
	physicsCollisions.onStart(manager);
	sdlRenderer.onStart(manager);
	soundFxPlayer.onStart(manager);

	int frames = 0;
	while (true && manager.getComponentOfType<Clock>(game)->currentTicks < 5000)
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
		weaponFiring.onUpdate(manager, inputs); // fire projectiles
		physicsDynamics.onUpdate(manager, inputs); // apply velocity to position
		boundariesChecker.onUpdate(manager, inputs); // apply boundaries or 
		physicsCollisions.onUpdate(manager, inputs); // check for collisions
		sdlRenderer.onUpdate(manager, inputs);
		soundFxPlayer.onUpdate(manager, inputs);

		++frames;
	}
	cerr << frames << endl;
	return 0;
}
