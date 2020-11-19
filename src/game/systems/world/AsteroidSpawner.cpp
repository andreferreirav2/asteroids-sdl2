#include "AsteroidSpawner.h"
#include "../../../engine/Math.h"
#include "../../components/AsteroidSpawnerParams.h"
#include "../../components/Clock.h"
#include "../../components/SpriteSDL.h"
#include "../../components/Transform.h"
#include "../../components/RigidBody.h"
#include "../../components/CircleCollider.h"
#include "../../components/ScoreAwarder.h"
#include <cmath>
#include <memory>
#include <string>
#include "../../components/BoundariesKill.h"
#include "../../components/PlayArea.h"
#include "../../components/Mesh.h"

using namespace std;

int const SMALL_ASTEROID = 0;
int const MEDIUM_ASTEROID = 1;
int const BIG_ASTEROID = 2;

void spawnAsteroid(ECSManager& manager, shared_ptr<AsteroidSpawnerParams> asteroidSpawn, int kind, float2 position, float rotation, float2 velocity);

void spawnChildAsteroids(ECSManager& manager, shared_ptr<AsteroidSpawnerParams> asteroidSpawn, int kind, shared_ptr<Transform> parentTransform, float2 velocity)
{
	int newAsts = randBetween(1, 3);
	for (float i = 0; i < newAsts; i++)
	{
		float randomAngle = randBetween(0, 360);
		auto newVel = velocity;
		newVel.x += cos(DEG_2_RAG * randomAngle) * randBetween(30, 60) * randSign();
		newVel.y += sin(DEG_2_RAG * randomAngle) * randBetween(30, 60) * randSign();
		spawnAsteroid(manager, asteroidSpawn, kind, parentTransform->position, randomAngle, newVel);
	}
}

void spawnAsteroid(ECSManager& manager, shared_ptr<AsteroidSpawnerParams> asteroidSpawn, int kind, float2 position, float rotation, float2 velocity)
{
	static auto asteroidSmallSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 16, 16 }), rect({ 0, 96, 64, 64 }));
	static auto asteroidMediumSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 40, 40 }), rect({ 64, 0, 160, 160 }));
	static auto asteroidLargeSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 64, 64 }), rect({ 224, 0, 288, 288 }));

	Entity ast = manager.createEntity();
	auto transform = make_shared<Transform>(position.x, position.y, rotation);
	manager.addComponent(ast, transform);
	manager.addComponent(ast, make_shared<BoundariesKill>());
	manager.addComponent(ast, make_shared<Mesh>(string("assets/models/monkey.obj")));

	if (kind == SMALL_ASTEROID) // small ast
	{
		manager.addComponent(ast, asteroidSmallSprite);
		manager.addComponent(ast, make_shared<ScoreAwarder>(100));
		manager.addComponent(ast, make_shared<RigidBody>(1.0f, 0.0f, velocity.x, velocity.y));
		manager.addComponent(ast, make_shared<CircleCollider>(8.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith, [&manager, ast](Entity other)
			{
				//TODO: Spawn destroy particles
				//manager.destroyEntity(ast);
			}));
	}
	else if (kind == MEDIUM_ASTEROID) // medium ast
	{
		manager.addComponent(ast, asteroidMediumSprite);
		manager.addComponent(ast, make_shared<ScoreAwarder>(50));
		manager.addComponent(ast, make_shared<RigidBody>(2.0f, 0.0f, velocity.x, velocity.y));
		manager.addComponent(ast, make_shared<CircleCollider>(20.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith, [&manager, ast, asteroidSpawn, transform, rotation, velocity](Entity other)
			{
				//TODO: Spawn destroy particles
				spawnChildAsteroids(manager, asteroidSpawn, SMALL_ASTEROID, transform, velocity);
				//manager.destroyEntity(ast);
			}));
	}
	else // big ast
	{
		manager.addComponent(ast, asteroidLargeSprite);
		manager.addComponent(ast, make_shared<ScoreAwarder>(20));
		manager.addComponent(ast, make_shared<RigidBody>(4.0f, 0.0f, velocity.x, velocity.y));
		manager.addComponent(ast, make_shared<CircleCollider>(32.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith, [&manager, ast, asteroidSpawn, transform, rotation, velocity](Entity other)
			{
				//TODO: Spawn destroy particles
				spawnChildAsteroids(manager, asteroidSpawn, MEDIUM_ASTEROID, transform, velocity);
				//manager.destroyEntity(ast);
			}));
	}
}

void AsteroidSpawner::onUpdate(ECSManager& manager, shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	rect playArea = manager.getAllComponentsOfType<PlayArea>().begin()->get()->area; // What if there is no play area?

	for (Entity e : manager.getAllEntitiesWithComponentType<AsteroidSpawnerParams>())
	{
		auto asteroidSpawn = manager.getComponentOfType<AsteroidSpawnerParams>(e);
		asteroidSpawn->timeToNextSpawn -= dt;

		if (asteroidSpawn->timeToNextSpawn <= 0)
		{
			asteroidSpawn->timeToNextSpawn = randBetween(asteroidSpawn->minInterval, asteroidSpawn->maxInterval);

			float totalRatios = asteroidSpawn->smallRatio + asteroidSpawn->mediumRatio + asteroidSpawn->bigRatio;
			float i = randBetween(0, totalRatios);

			// new asteroid parameters
			float rotation = randBetween(0, 360.0f);
			float2 position;
			float wall = randBetween(0, 4);
			if (wall < 1) // top
			{
				position = { randBetween(-asteroidSpawn->marginFromEdge, playArea.x + playArea.w + asteroidSpawn->marginFromEdge),
							 static_cast<float>(playArea.y - asteroidSpawn->marginFromEdge) };
			}
			else if (wall < 2) // bottom
			{
				position = { randBetween(-asteroidSpawn->marginFromEdge, playArea.x + playArea.w + asteroidSpawn->marginFromEdge),
							 static_cast<float>(playArea.y + playArea.h + asteroidSpawn->marginFromEdge) };
			}
			else if (wall < 3) // right
			{
				position = { static_cast<float>(playArea.x + playArea.w + asteroidSpawn->marginFromEdge),
							 randBetween(-asteroidSpawn->marginFromEdge, playArea.y + playArea.h + asteroidSpawn->marginFromEdge) };
			}
			else // left
			{
				position = { static_cast<float>(playArea.x - asteroidSpawn->marginFromEdge),
							  randBetween(-asteroidSpawn->marginFromEdge, playArea.y + playArea.h + asteroidSpawn->marginFromEdge) };
			}

			float2 randCenterPoint = { randBetween(playArea.x + playArea.w / 8, playArea.x + playArea.w * 7 / 8) ,
									   randBetween(playArea.y + playArea.h / 8, playArea.y + playArea.h * 7 / 8) };
			float2 velocity = { randCenterPoint.x - position.x, randCenterPoint.y - position.y };

			float magnitude = randBetween(0.04f, 0.3f);
			velocity.x *= magnitude;
			velocity.y *= magnitude;

			int asteroidKind;
			if (i < asteroidSpawn->smallRatio) // small ast
			{
				asteroidKind = SMALL_ASTEROID;
			}
			else if (i < asteroidSpawn->smallRatio + asteroidSpawn->mediumRatio) // medium ast
			{
				asteroidKind = MEDIUM_ASTEROID;
			}
			else // big ast
			{
				asteroidKind = BIG_ASTEROID;
			}

			spawnAsteroid(manager, asteroidSpawn, asteroidKind, position, rotation, velocity);
		}
	}
}
