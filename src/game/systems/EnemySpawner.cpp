#include "EnemySpawner.h"

#include "../components/Engine.h"
#include "../components/Transform.h"
#include "../components/RigidBody.h"
#include "../components/SpriteSDL.h"
#include "../components/SoundFXSDL.h"
#include "../components/CircleCollider.h"
#include <memory>
#include "../components/Weapon.h"
#include "../components/BoundariesKill.h"
#include "../components/ScoreAwarder.h"

using namespace std;

#define RAD_2_DEG 57.2958f
#define DEG_2_RAG 0.0174533f


void EnemySpawner::onStart(ECSManager& manager)
{
	// TODO Remove from here and reuse base ones
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


	auto shotSprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 2, 3 }), rect({ 0, 160, 32, 48 }));
	auto enemySprite = make_shared<SpriteSDL>(string("assets/sprites/atlas.png"), -90.0f, false, false, uint2({ 20, 12 }), rect({ 80, 160, 128, 60 }));


	Entity enemy = manager.createEntity();
	manager.addComponent(enemy, make_shared<Transform>(200, 200, 90.f));
	manager.addComponent(enemy, make_shared<RigidBody>(1.0f, 2.0f));
	manager.addComponent(enemy, std::make_shared<ScoreAwarder>(1000));
	manager.addComponent(enemy, enemySprite);
	manager.addComponent(enemy, make_shared<Engine>(300.0f, 150.f));
	manager.addComponent(enemy, make_shared<SoundFXSDL>(string("assets/audio/shoot.wav")));
	manager.addComponent(enemy, make_shared<CircleCollider>(7.0f, ENEMY_COLLIDER_LAYER, ENEMY_COLLIDES_WITH));
	manager.addComponent(enemy, make_shared<Weapon>(0.3f, [&manager, enemy, shotSprite, ENEMY_WEAPON_COLLIDER_LAYER, ENEMY_WEAPON_COLLIDES_WITH](shared_ptr<Transform> gun, shared_ptr<RigidBody> gunRb)
		{
			Entity shot = manager.createEntity();
			auto shotRb = std::make_shared<RigidBody>(1.0f, 0.0f);
			manager.addComponent(shot, std::make_shared<Transform>(gun->position.x, gun->position.y, gun->rotation));
			manager.addComponent(shot, std::make_shared<RigidBody>(1.0f, 0.0f, 200 * cos(gun->rotation * DEG_2_RAG), -200 * sin(gun->rotation * DEG_2_RAG)));
			manager.addComponent(shot, shotSprite);
			manager.addComponent(shot, std::make_shared<CircleCollider>(4.0f, ENEMY_WEAPON_COLLIDER_LAYER, ENEMY_WEAPON_COLLIDES_WITH));
			manager.addComponent(shot, make_shared<BoundariesKill>());
		}));
}

void EnemySpawner::onUpdate(ECSManager& manager, std::shared_ptr<Inputs> inputs)
{

}



