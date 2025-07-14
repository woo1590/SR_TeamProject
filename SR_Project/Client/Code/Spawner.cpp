#include "pch.h"
#include "Spawner.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "PoolingManager.h"

#include "Zombie.h"
#include "Skeleton.h"
#include "RedGolem.h"
#include "Creeper.h"
#include "TransformComponent.h"

Spawner* Spawner::Create(Scene* scene, ObjectManager* objMgr, SpawnType type, _vec3 pos, _vec3 rot)
{
	auto* instance = new Spawner(scene, objMgr, ObjectType::Neutral);
	instance->spawnType = type;
	instance->pos = pos;
	instance->rot = rot;
	return instance;
}

Monster* Spawner::Spawn()
{
	auto* pool = scene->GetPoolManager();
	Monster* monster = nullptr;

	switch (spawnType)
	{
	case SpawnType::Zombie:   monster = pool->Acquire<Zombie>(owner, ObjectType::Monster); break;
	case SpawnType::Skeleton: monster = pool->Acquire<Skeleton>(owner, ObjectType::Monster); break;
	case SpawnType::Creeper:  monster = pool->Acquire<Creeper>(owner, ObjectType::Monster); break;
	case SpawnType::RedGolem: monster = pool->Acquire<RedGolem>(owner, ObjectType::Monster); break;
	}

	//monster->Reset();

	auto tf = monster->GetComponent<TransformComponent>();
	tf->SetPosition(pos);
	tf->SetRotate(rot);

	owner->AddObject(ObjectType::Monster, monster);
	return monster;
}
