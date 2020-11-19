#include "EnemySpawner.h"
#include "../../../engine/Math.h"
#include "../../components/Engine.h"
#include "../../components/Transform.h"
#include "../../components/RigidBody.h"
#include "../../components/SpriteSDL.h"
#include "../../components/SoundFXSDL.h"
#include "../../components/CircleCollider.h"
#include <memory>
#include "../../components/Weapon.h"
#include "../../components/Boundless.h"
#include "../../components/BoundariesKill.h"
#include "../../components/ScoreAwarder.h"
#include "../../components/Clock.h"
#include "../../components/EnemySpawnerParams.h"
#include "../../components/DestroyAfterTime.h"
#include "../../components/ShipAIControls.h"
#include "../../components/PlayArea.h"
#include "../../components/Mesh.h"

using namespace std;


void EnemySpawner::onStart(ECSManager& manager)
{

}

void EnemySpawner::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{
	static auto enemySprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 20, 12 }), rect({ 80, 160, 128, 60 }));

	rect playArea = manager.getAllComponentsOfType<PlayArea>().begin()->get()->area; // What if there is no play area?
	float dt = manager.getAllComponentsOfType<Clock>().begin()->get()->deltaTime; // What if there is no clock?
	for (Entity e : manager.getAllEntitiesWithComponentType<EnemySpawnerParams>())
	{
		auto enemySpawn = manager.getComponentOfType<EnemySpawnerParams>(e);
		enemySpawn->timeToNextSpawn -= dt;

		if (enemySpawn->timeToNextSpawn <= 0)
		{
			enemySpawn->timeToNextSpawn = randBetween(enemySpawn->minInterval, enemySpawn->maxInterval);


			float2 position;
			float wall = randBetween(0, 4);
			if (wall < 1) // top
			{
				position = { randBetween(-enemySpawn->marginFromEdge, playArea.x + playArea.w + enemySpawn->marginFromEdge),
							 static_cast<float>(playArea.y - enemySpawn->marginFromEdge) };
			}
			else if (wall < 2) // bottom
			{
				position = { randBetween(-enemySpawn->marginFromEdge, playArea.x + playArea.w + enemySpawn->marginFromEdge),
							 static_cast<float>(playArea.y + playArea.h + enemySpawn->marginFromEdge) };
			}
			else if (wall < 3) // right
			{
				position = { static_cast<float>(playArea.x + playArea.w + enemySpawn->marginFromEdge),
							 randBetween(-enemySpawn->marginFromEdge, playArea.y + playArea.h + enemySpawn->marginFromEdge) };
			}
			else // left
			{
				position = { static_cast<float>(playArea.x - enemySpawn->marginFromEdge),
							  randBetween(-enemySpawn->marginFromEdge, playArea.y + playArea.h + enemySpawn->marginFromEdge) };
			}


			Entity enemyShip = manager.createEntity();
			manager.addComponent(enemyShip, make_shared<Transform>(position.x, position.y, 90.f));
			manager.addComponent(enemyShip, make_shared<RigidBody>(1.0f, 1.2f));
			manager.addComponent(enemyShip, std::make_shared<ScoreAwarder>(1000));
			manager.addComponent(enemyShip, std::make_shared<Boundless>());
			manager.addComponent(enemyShip, std::make_shared<ShipAIControls>());
			manager.addComponent(enemyShip, enemySprite);
			manager.addComponent(enemyShip, make_shared<Mesh>(string("assets/models/monkey.obj"), 7.0f, -90.0f));
			manager.addComponent(enemyShip, make_shared<Engine>(300.0f, 70.f));
			manager.addComponent(enemyShip, make_shared<SoundFXSDL>(string("assets/audio/shoot.wav")));
			manager.addComponent(enemyShip, make_shared<CircleCollider>(7.0f, enemySpawn->enemyColliderLayer, enemySpawn->enemyCollidesWith));
			manager.addComponent(enemyShip, make_shared<Weapon>(1.0f, [&manager, enemyShip, enemySpawn](shared_ptr<Transform> gun, shared_ptr<RigidBody> gunRb)
				{
					Entity shot = manager.createEntity();
					manager.addComponent(shot, make_shared<Mesh>(string("assets/models/bullet.obj"), 2.0f));
					manager.addComponent(shot, std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation));
					manager.addComponent(shot, std::make_shared<RigidBody>(1.0f, 0.0f, 200 * cos(gun->rotation * DEG_2_RAG), -200 * sin(gun->rotation * DEG_2_RAG)));
					manager.addComponent(shot, std::make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 4, 6 }), rect({ 0, 160, 32, 48 })));
					manager.addComponent(shot, std::make_shared<CircleCollider>(4.0f, enemySpawn->enemyWeaponColliderLayer, enemySpawn->enemyWeaponCollidesWith, [&manager, shot](Entity other)
						{
							manager.destroyEntity(shot);
						}));

					manager.addComponent(shot, make_shared<Boundless>());
					manager.addComponent(shot, make_shared<DestroyAfterTime>(2));
				}));
		}
	}
}
