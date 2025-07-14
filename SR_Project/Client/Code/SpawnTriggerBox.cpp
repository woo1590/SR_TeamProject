#include "pch.h"
#include "SpawnTriggerBox.h"

#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "Scene.h"
#include "Spawner.h"

SpawnTriggerBox::SpawnTriggerBox(ObjectManager* owner, ObjectType objType)
	:Object(owner, objType)
{
}

SpawnTriggerBox::~SpawnTriggerBox()
{
}

SpawnTriggerBox* SpawnTriggerBox::Create(ObjectManager* owner, ObjectType objType)
{
    SpawnTriggerBox* Instance = new SpawnTriggerBox(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT SpawnTriggerBox::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto collision = AddComponent<CollisionComponent>();
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(50.f, 0.f, 50.f);

    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_TRRIGER);
    collision->SetMask(LAYER_PLAYER);       
    collision->SetCollisionEnter([this](Object* other) {this->OnCollisionEnter(other); });
    collision->SetSize(_vec3(10.f, 10.f, 10.f));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetKinematic(true);
    physics->SetMass(1.f);
    return S_OK;
}

void SpawnTriggerBox::Update(_float dt)
{
    Object::Update(dt);
}

void SpawnTriggerBox::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void SpawnTriggerBox::SetCollisionSize(_vec3 size)
{
    auto collision = GetComponent<CollisionComponent>();
    if (collision)
    {
        collision->SetSize(size);
    }
}

void SpawnTriggerBox::SetTriggerPosition(_vec3 pos)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(pos);
}

void SpawnTriggerBox::AddSpawner(SpawnType type, _vec3 pos, _vec3 rot)
{
    spawners.push_back(Spawner::Create(owner, ObjectType::Neutral, type, pos, rot));
}

void SpawnTriggerBox::OnCollisionEnter(Object* other)
{
    if (SpawnOn) return;

    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    if (objType == ObjectType::Player)
    {
        for (auto& spawner : spawners)
        {
            spawner->Spawn();
        }
        SpawnOn = true;
    }
}

void SpawnTriggerBox::Free()
{
    Object::Free();

    std::for_each(spawners.begin(), spawners.end(), [](Object* spawner)
        {
            Safe_Release(spawner);
        });
}
