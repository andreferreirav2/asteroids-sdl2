#include "AsteroidSpawner.h"
#include "../components/AsteroidSpawnerParams.h"
#include "../components/Clock.h"
#include "../components/SpriteSDL.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include "../components/CircleCollider.h"
#include <cmath>
#include <memory>
#include <string>
#include <cstdlib>

int const SMALL_ASTEROID = 0;
int const MEDIUM_ASTEROID = 1;
int const BIG_ASTEROID = 2;

#define DEG_2_RAG 0.0174533f

float randBetween(float a, float b)
{
	float r = (float)rand() / (RAND_MAX);
	return a + (b - a) * r;
}

void spawnAsteroid(ECSManager& manager, std::shared_ptr<AsteroidSpawnerParams> asteroidSpawn, int kind, float2 position, float rotation, float2 velocity)
{
	static auto asteroidSmallSprite = std::make_shared<SpriteSDL>(std::string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 16, 16 }), rect({ 0, 96, 64, 64 }));
	static auto asteroidMediumSprite = std::make_shared<SpriteSDL>(std::string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 40, 40 }), rect({ 64, 0, 160, 160 }));
	static auto asteroidLargeSprite = std::make_shared<SpriteSDL>(std::string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 64, 64 }), rect({ 224, 0, 288, 288 }));

	Entity ast = manager.createEntity();
	auto transform = std::make_shared<Transform>(position.x, position.y, rotation);
	manager.addComponent(ast, transform);

	if (kind == SMALL_ASTEROID) // small ast
	{
		manager.addComponent(ast, asteroidSmallSprite);
		manager.addComponent(ast, std::make_shared<RigidBody>(1.0f, 0.0f, velocity.x, velocity.y));
		manager.addComponent(ast, std::make_shared<CircleCollider>(8.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith));
	}
	else if (kind == MEDIUM_ASTEROID) // medium ast
	{
		manager.addComponent(ast, asteroidMediumSprite);
		manager.addComponent(ast, std::make_shared<RigidBody>(2.0f, 0.0f, velocity.x, velocity.y));
		manager.addComponent(ast, std::make_shared<CircleCollider>(20.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith, [&manager, asteroidSpawn, transform, rotation, velocity](Entity other)
			{
				int newAsts = static_cast<int>(randBetween(2, 5));
				float randomAngle = randBetween(0, 360.0f);
				for (int i = 0; i < newAsts; i++)
				{
					auto newPos = transform->position;
					newPos.x += cos(DEG_2_RAG * (randomAngle + 360 * ((i + 1) / newAsts))) * 30;
					newPos.y += sin(DEG_2_RAG * (randomAngle + 360 * ((i + 1) / newAsts))) * 30;

					auto newVel = velocity;
					newVel.x += (newPos.x - transform->position.x) / 2;
					newVel.y += (newPos.y - transform->position.y) / 2;
					spawnAsteroid(manager, asteroidSpawn, SMALL_ASTEROID, newPos, randomAngle, newVel);
				}
			}));
	}
	else // big ast
	{
		manager.addComponent(ast, asteroidLargeSprite);
		manager.addComponent(ast, std::make_shared<RigidBody>(4.0f, 0.0f, velocity.x, velocity.y));
		manager.addComponent(ast, std::make_shared<CircleCollider>(32.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith, [&manager, asteroidSpawn, transform, rotation, velocity](Entity other)
			{
				int newAsts = static_cast<int>(randBetween(2, 5));
				float randomAngle = randBetween(0, 360.0f);
				for (int i = 0; i < newAsts; i++)
				{
					auto newPos = transform->position;
					newPos.x += cos(DEG_2_RAG * (randomAngle + 360 * ((i + 1) / newAsts))) * 30;
					newPos.y += sin(DEG_2_RAG * (randomAngle + 360 * ((i + 1) / newAsts))) * 30;

					auto newVel = velocity;
					newVel.x = (newVel.x + newPos.x - transform->position.x) / 2;
					newVel.y = (newVel.y + newPos.y - transform->position.y) / 2;
					spawnAsteroid(manager, asteroidSpawn, MEDIUM_ASTEROID, newPos, randomAngle, newVel);
				}
			}));
	}
}

void AsteroidSpawner::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
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
				position = { randBetween(-50, asteroidSpawn->playArea.x + asteroidSpawn->playArea.w + 50.0f), asteroidSpawn->playArea.y - 50.0f };
			}
			else if (wall < 2) // bottom
			{
				position = { randBetween(-50, asteroidSpawn->playArea.x + asteroidSpawn->playArea.w + 50.0f), asteroidSpawn->playArea.y + asteroidSpawn->playArea.h + 50.0f };
			}
			else if (wall < 3) // right
			{
				position = { asteroidSpawn->playArea.x + asteroidSpawn->playArea.w + 50.0f, randBetween(-50, asteroidSpawn->playArea.y + asteroidSpawn->playArea.h + 50.0f) };
			}
			else // left
			{
				position = { asteroidSpawn->playArea.x - 50.0f, randBetween(-50, asteroidSpawn->playArea.y + asteroidSpawn->playArea.h + 50.0f) };
			}

			float2 randCenterPoint = { randBetween(asteroidSpawn->playArea.x + asteroidSpawn->playArea.w / 8, asteroidSpawn->playArea.x + asteroidSpawn->playArea.w * 7 / 8) ,
									   randBetween(asteroidSpawn->playArea.y + asteroidSpawn->playArea.h / 8, asteroidSpawn->playArea.y + asteroidSpawn->playArea.h * 7 / 8) };
			float2 velocity = { randCenterPoint.x - position.x, randCenterPoint.y - position.y };

			float magnitude = randBetween(0.02f, 0.2f);
			velocity.x *= magnitude;
			velocity.y *= magnitude;

			if (i < asteroidSpawn->smallRatio) // small ast
			{
				spawnAsteroid(manager, asteroidSpawn, SMALL_ASTEROID, position, rotation, velocity);
			}
			else if (i < asteroidSpawn->smallRatio + asteroidSpawn->mediumRatio) // medium ast
			{
				spawnAsteroid(manager, asteroidSpawn, MEDIUM_ASTEROID, position, rotation, velocity);
			}
			else // big ast
			{
				spawnAsteroid(manager, asteroidSpawn, BIG_ASTEROID, position, rotation, velocity);
			}
		}
	}
}
