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
#include "components/DestroyAfterTime.h"
#include "components/BoundariesKill.h"
#include "components/Weapon.h"
#include "components/SecondaryWeapon.h"
#include "components/Clock.h"
#include "components/Score.h"
#include "components/ScoreAwarder.h"
#include "components/AsteroidSpawnerParams.h"
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
#include "systems/AsteroidSpawner.h"
#include "systems/EnemySpawner.h"
#include "systems/WeaponFiring.h"
#include "systems/DestroyAfterEntitiesTime.h"
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <typeinfo>
#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include "components/Respawn.h"
#include "components/Lives.h"

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

	// Seed random with time
	srand(time(NULL));

	// Pre load assets
	app.loadTexture(string("assets/sprites/atlas.png"));
	app.getMusic(string("assets/audio/shoot.wav"));
	app.loadFont(string("assets/fonts/Roboto-Regular.ttf"), 28);

	auto shipSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 16, 24 }), rect({ 0, 0, 64, 96 }));
	auto shotSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 2, 3 }), rect({ 0, 160, 32, 48 }));
	auto mineSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0, false, false, uint2({ 12, 12 }), rect({ 32, 160, 48, 48 }));
	auto explosionSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0, false, false, uint2({ 12, 12 }), rect({ 0, 288, 224, 224 }));

	ECSManager manager;

	Entity game = manager.createEntity();
	manager.addComponent(game, make_shared<Clock>(1.0f, 1000));
	manager.addComponent(game, make_shared<AsteroidSpawnerParams>(1.0f, 2.0f, 0.1f, 0.5f, 0.4f, ASTEROIDS_COLLIDER_LAYER, ASTEROIDS_COLLIDES_WITH, rect({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT })));

	int players = 4;
	for (int i = 0; i < players; i++)
	{
		float2 spawn = { SCREEN_WIDTH / 2 + (-100 * (players - 1) + 200 * i), SCREEN_HEIGHT / 2 };

		Entity ship = manager.createEntity();
		auto shipTransform = make_shared<Transform>(spawn.x, spawn.y, 90.f);
		auto shipRb = make_shared<RigidBody>(1.0f, 1.2f);
		auto scoreBoard = make_shared<Score>();
		manager.addComponent(ship, shipTransform);
		manager.addComponent(ship, shipRb);
		manager.addComponent(ship, shipSprite);
		manager.addComponent(ship, scoreBoard);
		manager.addComponent(ship, make_shared<Respawn>(spawn, 90.0f));
		manager.addComponent(ship, make_shared<Lives>(3));
		manager.addComponent(ship, make_shared<Engine>(400.0f, 150.f));
		manager.addComponent(ship, make_shared<ShipManualControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE, Key::KEY_DOWN));
		manager.addComponent(ship, make_shared<Boundless>());
		manager.addComponent(ship, make_shared<SoundFXSDL>(string("assets/audio/shoot.wav")));
		manager.addComponent(ship, make_shared<CircleCollider>(7.0f, PLAYER_COLLIDER_LAYER, PLAYER_COLLIDES_WITH, [&manager, ship](Entity other)
			{
				auto respawn = manager.getComponentOfType<Respawn>(ship);
				if (respawn)
				{
					auto shipTransform = manager.getComponentOfType<Transform>(ship);
					auto shipRb = manager.getComponentOfType<RigidBody>(ship);

					shipTransform->position = respawn->position;
					shipTransform->rotation = respawn->rotation;
					shipRb->velocity = { 0.0f, 0.0f };
				}

				auto lives = manager.getComponentOfType<Lives>(ship);
				if (lives && --lives->left < 0)
				{
					cerr << "Game over! lives: " << lives->left << endl;
					// TODO: gameover
				}

				manager.destroyEntity(other);
			}));
		manager.addComponent(ship, make_shared<Weapon>(0.3f, [&manager, shotSprite, ship](shared_ptr<Transform> gun, shared_ptr<RigidBody> gunRb)
			{
				Entity shot = manager.createEntity();
				auto shotRb = std::make_shared<RigidBody>(1.0f, 0.0f);
				manager.addComponent(shot, std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation));
				manager.addComponent(shot, std::make_shared<RigidBody>(1.0f, 0.0f, 300 * cos(gun->rotation * DEG_2_RAG), -300 * sin(gun->rotation * DEG_2_RAG)));
				manager.addComponent(shot, shotSprite);
				manager.addComponent(shot, make_shared<BoundariesKill>());
				manager.addComponent(shot, std::make_shared<CircleCollider>(4.0f, PLAYER_WEAPON_COLLIDER_LAYER, PLAYER_WEAPON_COLLIDES_WITH, [&manager, ship, shot](Entity other)
					{
						auto scoreBoard = manager.getComponentOfType<Score>(ship);
						auto scoreAwarder = manager.getComponentOfType<ScoreAwarder>(other);
						if (scoreBoard && scoreAwarder)
						{
							scoreBoard->score += scoreAwarder->score;
						}

						manager.destroyEntity(other);
						manager.destroyEntity(shot);
					}));
			}));
		manager.addComponent(ship, make_shared<SecondaryWeapon>(1.0f, 0, [&manager, mineSprite, explosionSprite, ship](shared_ptr<Transform> gun, shared_ptr<RigidBody> gunRb)
			{
				Entity mine = manager.createEntity();
				auto mineTransform = std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation);
				manager.addComponent(mine, mineTransform);
				manager.addComponent(mine, mineSprite);
				manager.addComponent(mine, make_shared<Boundless>());
				manager.addComponent(mine, make_shared<DestroyAfterTime>(10.0f));
				manager.addComponent(mine, std::make_shared<RigidBody>(1.0f, 0.3f, gunRb->velocity.x, gunRb->velocity.y));
				manager.addComponent(mine, std::make_shared<CircleCollider>(10.0f, PLAYER_WEAPON_COLLIDER_LAYER, PLAYER_WEAPON_COLLIDES_WITH, [&manager, ship, mine, explosionSprite, mineTransform](Entity other)
					{
						// TODO: play explosion sound
						// TODO: play explosion animation

						// Spawn explosion ball
						Entity explosion = manager.createEntity();
						manager.addComponent(explosion, std::make_shared<Transform>(mineTransform->position.x, mineTransform->position.y, mineTransform->rotation, 20.0f, 20.0f));
						manager.addComponent(explosion, explosionSprite);
						manager.addComponent(explosion, make_shared<BoundariesKill>());
						manager.addComponent(explosion, make_shared<DestroyAfterTime>(0.15f));
						manager.addComponent(explosion, std::make_shared<CircleCollider>(100.0f, PLAYER_WEAPON_COLLIDER_LAYER, PLAYER_WEAPON_COLLIDES_WITH, [&manager, ship](Entity other)
							{
								auto scoreBoard = manager.getComponentOfType<Score>(ship);
								auto scoreAwarder = manager.getComponentOfType<ScoreAwarder>(other);
								if (scoreBoard && scoreAwarder)
								{
									scoreBoard->score += scoreAwarder->score;
								}
								manager.destroyEntity(other);
							}));

						manager.destroyEntity(mine);
					}));
			}));
	}

	TimePassing timePassing = TimePassing();
	ShipKeyboardController shipKeyboardController = ShipKeyboardController();
	ShipGameController shipGameController = ShipGameController();
	EnginesThrusters enginesThrusters = EnginesThrusters();
	WeaponFiring weaponFiring = WeaponFiring();
	PhysicsDynamics physicsDynamics = PhysicsDynamics();
	PhysicsCollisions physicsCollisions = PhysicsCollisions();
	AsteroidSpawner asteroidSpawner = AsteroidSpawner();
	EnemySpawner enemySpawner = EnemySpawner();
	DestroyAfterEntitiesTime destroyAfterEntitiesTime = DestroyAfterEntitiesTime();
	BoundariesChecker boundariesChecker = BoundariesChecker({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SDLRenderer sdlRenderer = SDLRenderer(app);
	SoundFxPlayer soundFxPlayer = SoundFxPlayer(app);

	timePassing.onStart(manager);
	shipKeyboardController.onStart(manager);
	shipGameController.onStart(manager);
	enginesThrusters.onStart(manager);
	weaponFiring.onStart(manager);
	physicsDynamics.onStart(manager);
	destroyAfterEntitiesTime.onStart(manager);
	boundariesChecker.onStart(manager);
	physicsCollisions.onStart(manager);
	asteroidSpawner.onStart(manager);
	enemySpawner.onStart(manager);
	sdlRenderer.onStart(manager);
	soundFxPlayer.onStart(manager);

	int frames = 0;
	//while (true && manager.getComponentOfType<Clock>(game)->currentTicks < 5000)
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
		weaponFiring.onUpdate(manager, inputs); // fire projectiles
		physicsDynamics.onUpdate(manager, inputs); // apply velocity to position
		destroyAfterEntitiesTime.onUpdate(manager, inputs); // destroy objects after time
		boundariesChecker.onUpdate(manager, inputs); // apply boundaries or 
		physicsCollisions.onUpdate(manager, inputs); // check for collisions
		asteroidSpawner.onUpdate(manager, inputs); // spawn new asteroids
		enemySpawner.onUpdate(manager, inputs); // spawn new enemies
		sdlRenderer.onUpdate(manager, inputs);
		soundFxPlayer.onUpdate(manager, inputs);

		++frames;
	}
	cerr << frames << endl;
	//cin.ignore();
	return 0;
}
