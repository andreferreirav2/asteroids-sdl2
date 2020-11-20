#include "GameParams.h"
#include "../engine/sdl/SDLApp.h"
#include "../engine/ecs/ECSManager.h"
#include "../engine/input/Inputs.h"
#include "../engine/Types.h"
#include "../engine/Math.h"
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
#include "components/EnemySpawnerParams.h"
#include "components/CircleCollider.h"
#include "systems/world/PhysicsDynamics.h"
#include "systems/game/SDL2DRenderer.h"
#include "systems/game/SDL3DRenderer.h"
#include "systems/game/ShipKeyboardController.h"
#include "systems/game/ShipGameController.h"
#include "systems/game/SoundFxPlayer.h"
#include "systems/combat/EnginesThrusters.h"
#include "systems/world/BoundariesChecker.h"
#include "systems/game/TimePassing.h"
#include "systems/world/PhysicsCollisions.h"
#include "systems/world/AsteroidSpawner.h"
#include "systems/combat/EnemySpawner.h"
#include "systems/combat/WeaponFiring.h"
#include "systems/game/DestroyAfterEntitiesTime.h"
#include "systems/combat/EnemyAIController.h"
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
#include "components/Player.h"
#include "components/PlayArea.h"
#include "components/Mesh.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const bool OPENGL = true;

int PLAYER_COLLIDER_LAYER = 1 << 0;
int PLAYER_WEAPON_COLLIDER_LAYER = 1 << 1;
int ENEMY_COLLIDER_LAYER = 1 << 2;
int ENEMY_WEAPON_COLLIDER_LAYER = 1 << 3;
int ASTEROIDS_COLLIDER_LAYER = 1 << 4;

int PLAYER_COLLIDES_WITH = ENEMY_COLLIDER_LAYER | ENEMY_WEAPON_COLLIDER_LAYER | ASTEROIDS_COLLIDER_LAYER;
int PLAYER_WEAPON_COLLIDES_WITH = ENEMY_COLLIDER_LAYER | ASTEROIDS_COLLIDER_LAYER;
int ENEMY_COLLIDES_WITH = PLAYER_COLLIDER_LAYER | PLAYER_WEAPON_COLLIDER_LAYER;
int ENEMY_WEAPON_COLLIDES_WITH = PLAYER_COLLIDER_LAYER;// | ASTEROIDS_COLLIDER_LAYER;
int ASTEROIDS_COLLIDES_WITH = PLAYER_COLLIDER_LAYER | PLAYER_WEAPON_COLLIDER_LAYER;// | ENEMY_WEAPON_COLLIDER_LAYER;

int main(int argc, char* args[])
{

	SDLApp app = SDLApp(SCREEN_WIDTH, SCREEN_HEIGHT, OPENGL, "assets/shaders/vertex.fx", "assets/shaders/fragment.fx");
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
	app.loadObjFileGL(string("assets/models/monkey.obj"));
	app.loadObjFileGL(string("assets/models/asteroid.obj"));
	app.loadObjFileGL(string("assets/models/bullet.obj"));
	app.loadObjFileGL(string("assets/models/ship.obj"));
	app.loadObjFileGL(string("assets/models/enemyship.obj"));

	auto shipSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 12, 18 }), rect({ 0, 0, 64, 96 }));
	auto shotSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 2, 3 }), rect({ 0, 160, 32, 48 }));
	auto mineSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0, false, false, uint2({ 12, 12 }), rect({ 32, 160, 48, 48 }));
	auto explosionSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0, false, false, uint2({ 12, 12 }), rect({ 0, 288, 224, 224 }));

	ECSManager manager;

	Entity game = manager.createEntity();
	manager.addComponent(game, make_shared<PlayArea>(rect({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT })));
	manager.addComponent(game, make_shared<Clock>(1.0f, 1000));
	manager.addComponent(game, make_shared<AsteroidSpawnerParams>(ASTEROID_SPAWN_PER_MINUTE_BASE, 50, 0.1f, 0.5f, 0.4f, ASTEROIDS_COLLIDER_LAYER, ASTEROIDS_COLLIDES_WITH));
	manager.addComponent(game, make_shared<EnemySpawnerParams>(ENEMY_SPAWN_PER_MINUTE_BASE, 0, ENEMY_COLLIDER_LAYER, ENEMY_COLLIDES_WITH, ENEMY_WEAPON_COLLIDER_LAYER, ENEMY_WEAPON_COLLIDES_WITH));

	int players = NUMBER_OF_PLAYERS;
	for (int i = 0; i < players; i++)
	{
		float2 spawn = { SCREEN_WIDTH / 2 + (-100 * (players - 1) + 200 * i), SCREEN_HEIGHT / 2 };

		Entity ship = manager.createEntity();
		auto shipTransform = make_shared<Transform>(spawn.x, spawn.y, 90.f);
		auto shipRb = make_shared<RigidBody>(1.0f, 1.2f);
		auto scoreBoard = make_shared<Score>();
		manager.addComponent(ship, shipTransform);
		manager.addComponent(ship, shipRb);
		manager.addComponent(ship, scoreBoard);
		manager.addComponent(ship, shipSprite);
		manager.addComponent(ship, make_shared<Mesh>(string("assets/models/ship.obj"), 7.0f, 0.0f, float3{ 0.4f, 0.4f, 1.0f }, 0.6));
		manager.addComponent(ship, make_shared<Respawn>(spawn, 90.0f));
		manager.addComponent(ship, make_shared<Lives>(3));
		manager.addComponent(ship, make_shared<Engine>(400.0f, 150.f));
		manager.addComponent(ship, make_shared<ShipManualControls>(Key::KEY_UP, Key::KEY_LEFT, Key::KEY_RIGHT, Key::KEY_SPACE, Key::KEY_DOWN));
		manager.addComponent(ship, make_shared<Boundless>());
		manager.addComponent(ship, make_shared<Player>());
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
				auto scoreBoard = manager.getComponentOfType<Score>(ship);
				if (scoreBoard)
				{
					scoreBoard->score = max(0, scoreBoard->score - PRIMARY_WEAPON_COST);
				}

				Entity shot = manager.createEntity();
				auto shotRb = std::make_shared<RigidBody>(1.0f, 0.0f);
				manager.addComponent(shot, std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation));
				manager.addComponent(shot, std::make_shared<RigidBody>(1.0f, 0.0f, PRIMARY_WEAPON_AMMO_SPEED * cos(gun->rotation * DEG_2_RAG), -PRIMARY_WEAPON_AMMO_SPEED * sin(gun->rotation * DEG_2_RAG)));
				manager.addComponent(shot, shotSprite);
				manager.addComponent(shot, make_shared<Mesh>(string("assets/models/bullet.obj"), 2.0f, 0.0f, float3{ 1.0f, 1.0f, 1.0f }, 0.9));
				manager.addComponent(shot, make_shared<Boundless>());
				manager.addComponent(shot, make_shared<DestroyAfterTime>(1.0f));
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
				auto scoreBoard = manager.getComponentOfType<Score>(ship);
				if (!(scoreBoard && (scoreBoard->score >= SECONDARY_WEAPON_COST)))
				{
					return;
				}

				scoreBoard->score = max(0, scoreBoard->score - SECONDARY_WEAPON_COST);

				Entity mine = manager.createEntity();
				auto mineTransform = std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation);
				manager.addComponent(mine, mineTransform);
				manager.addComponent(mine, mineSprite);
				manager.addComponent(mine, make_shared<Mesh>(string("assets/models/monkey.obj"), 10.0f));
				manager.addComponent(mine, make_shared<Boundless>());
				//manager.addComponent(mine, make_shared<DestroyAfterTime>(30.0f));
				manager.addComponent(mine, std::make_shared<RigidBody>(1.0f, 0.3f, gunRb->velocity.x, gunRb->velocity.y));
				manager.addComponent(mine, std::make_shared<CircleCollider>(10.0f, PLAYER_WEAPON_COLLIDER_LAYER, PLAYER_WEAPON_COLLIDES_WITH, [&manager, ship, mine, explosionSprite, mineTransform](Entity other)
					{
						// TODO: play explosion sound
						// TODO: play explosion animation

						// Spawn explosion ball
						Entity explosion = manager.createEntity();
						manager.addComponent(explosion, std::make_shared<Transform>(mineTransform->position.x, mineTransform->position.y, mineTransform->rotation, 20.0f, 20.0f));
						manager.addComponent(explosion, explosionSprite);
						manager.addComponent(explosion, make_shared<Mesh>(string("assets/models/monkey.obj"), SECONDARY_WEAPON_AMMO_RANGE));
						manager.addComponent(explosion, make_shared<BoundariesKill>());
						manager.addComponent(explosion, make_shared<DestroyAfterTime>(0.15f));
						manager.addComponent(explosion, std::make_shared<CircleCollider>(SECONDARY_WEAPON_AMMO_RANGE, PLAYER_WEAPON_COLLIDER_LAYER, PLAYER_WEAPON_COLLIDES_WITH, [&manager, ship](Entity other)
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

	auto systems = vector<shared_ptr<System>>{
		make_shared<TimePassing>(),
		make_shared<ShipKeyboardController>(),
		make_shared<ShipGameController>(),
		make_shared<EnginesThrusters>(),
		make_shared<WeaponFiring>(),
		make_shared<PhysicsDynamics>(),
		make_shared<PhysicsCollisions>(),
		make_shared<AsteroidSpawner>(),
		make_shared<EnemySpawner>(),
		make_shared<EnemyAIController>(),
		make_shared<DestroyAfterEntitiesTime>(),
		make_shared<BoundariesChecker>(),
		make_shared<SDL2DRenderer>(app),
		make_shared<SDL3DRenderer>(app),
		make_shared<SoundFxPlayer>(app),
	};

	for (auto const& system : systems)
	{
		system->onStart(manager);
	}

	int frames = 0;
	//while (true && manager.getComponentOfType<Clock>(game)->currentTicks < 5000)
	while (true)
	{
		auto inputs = app.parseInputs(); // parse inputs from SDL
		if (inputs->isPressed(Key::QUIT))
		{
			break;
		}
		else if (inputs->isPressed(Key::KEY_2))
		{
			app.setOpenGL(false);
		}
		else if (inputs->isPressed(Key::KEY_3))
		{
			app.setOpenGL(true);
		}

		for (auto const& system : systems)
		{
			system->onUpdate(manager, inputs);
		}

		++frames;
	}
	cerr << frames << endl;
	//cin.ignore();
	return 0;
}
