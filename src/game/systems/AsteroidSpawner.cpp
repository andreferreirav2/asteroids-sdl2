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

float randBetween(float a, float b)
{
	float r = (float)rand() / (RAND_MAX);
	return a + (b - a) * r;
}

void AsteroidSpawner::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	static auto asteroidSmallSprite = std::make_shared<SpriteSDL>(std::string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 16, 16 }), rect({ 0, 96, 64, 64 }));
	static auto asteroidMediumSprite = std::make_shared<SpriteSDL>(std::string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 40, 40 }), rect({ 64, 0, 160, 160 }));
	static auto asteroidLargeSprite = std::make_shared<SpriteSDL>(std::string("assets/sprites/atlas.png"), 0.0f, false, false, uint2({ 64, 64 }), rect({ 224, 0, 288, 288 }));

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

			Entity ast = manager.createEntity();
			manager.addComponent(ast, std::make_shared<Transform>(position.x, position.y, rotation));
			if (i < asteroidSpawn->smallRatio) // small ast
			{
				manager.addComponent(ast, asteroidSmallSprite);
				manager.addComponent(ast, std::make_shared<RigidBody>(1.0f, 0.0f, velocity.x, velocity.y));
				manager.addComponent(ast, std::make_shared<CircleCollider>(8.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith));
			}
			else if (i < asteroidSpawn->smallRatio + asteroidSpawn->mediumRatio) // medium ast
			{
				manager.addComponent(ast, asteroidMediumSprite);
				manager.addComponent(ast, std::make_shared<RigidBody>(2.0f, 0.0f, velocity.x, velocity.y));
				manager.addComponent(ast, std::make_shared<CircleCollider>(20.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith));
			}
			else // big ast
			{
				manager.addComponent(ast, asteroidLargeSprite);
				manager.addComponent(ast, std::make_shared<RigidBody>(4.0f, 0.0f, velocity.x, velocity.y));
				manager.addComponent(ast, std::make_shared<CircleCollider>(32.0f, asteroidSpawn->asteroidsColliderLayer, asteroidSpawn->asteroidsCollidesWith));
			}
		}
	}
}
