#include "pch.h"
#include "Spawner.h"
#include "Monster.h"
#include "ObjectManager.h"

#include "Zombie.h"
#include "Skeleton.h"
#include "RedGolem.h"
#include "Creeper.h"
#include "JungleZombie.h"
#include "Slime.h"
#include "PurpleSlime.h"
#include "Ender.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

Spawner::Spawner(ObjectManager* owner, ObjectType objType)
    :Object(owner, objType)
{
}

Spawner::~Spawner()
{
}

Spawner* Spawner::Create(ObjectManager* owner, ObjectType objType, SpawnType type, _vec3 position, _vec3 rotation)
{
    Spawner* Instance = new Spawner(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType, type, position, rotation)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Spawner::Ready_Object(ObjectManager* owner, ObjectType objType, SpawnType type, _vec3 position, _vec3 rotation)
{
    spawntype = type;
    Position = position;
    Rotation = rotation;

    return S_OK;
}

Monster* Spawner::Spawn()
{
    Monster* monster = nullptr;
    switch (spawntype)
    {
    case SpawnType::Zombie:
        monster = Zombie::Create(owner, ObjectType::Monster);
        break; 

    case SpawnType::Creeper:
        monster = Creeper::Create(owner, ObjectType::Monster);
        break;

    case SpawnType::Skeleton:
        monster = Skeleton::Create(owner, ObjectType::Monster);
        break;

    case SpawnType::RedGolem:
        monster = RedGolem::Create(owner, ObjectType::Monster);
        break;

    case SpawnType::Slime:
        monster = Slime::Create(owner, ObjectType::Monster);
        break;

    case SpawnType::PurpleSlime:
        monster = PurpleSlime::Create(owner, ObjectType::Monster);
        break;

    case SpawnType::JungleZombie:
        monster = JungleZombie::Create(owner, ObjectType::Monster);
        break;

    case SpawnType::Ender:
        monster = Ender::Create(owner, ObjectType::Monster);
        break;
    }

    auto transform = monster->GetComponent<TransformComponent>();

    if (transform)
    {
        transform->SetPosition(Position);

        if (spawntype == SpawnType::Ender)
        {
            static_cast<Ender*>(monster)->SetTargetPos(Position);
            monster->GetComponent<PhysicsComponent>()->SetGround(false);
        }

        transform->SetRotate(Rotation);
    }

    owner->AddObject(ObjType, monster);

    return monster;
}
