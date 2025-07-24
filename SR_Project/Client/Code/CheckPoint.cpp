#include "pch.h"
#include "CheckPoint.h"

#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "Scene.h"
#include "Player.h"

CheckPoint::CheckPoint(ObjectManager* owner, ObjectType objType, LOADID _scene, _int _num)
	:Object(owner, objType)
{
    scene = _scene;
    num = _num;
}

CheckPoint::~CheckPoint()
{
}

CheckPoint* CheckPoint::Create(ObjectManager* owner, ObjectType objType, LOADID _scene, _int _num)
{
    CheckPoint* Instance = new CheckPoint(owner, objType,_scene,_num);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT CheckPoint::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<AABBCollider>();
    auto transform = AddComponent<TransformComponent>();
    transform->SetPosition(50.f, 0.f, 50.f);

    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(LAYER_TRRIGER);
    collision->SetMask(LAYER_PLAYER);
    collision->SetCollisionEnter([this](Object* other) {this->OnCollisionEnter(other); });
    collision->SetSize(_vec3(20.f, 20.f, 20.f));

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetKinematic(true);
    physics->SetMass(1.f);
    return S_OK;
}

void CheckPoint::Update(_float dt)
{
    Object::Update(dt);
}

void CheckPoint::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void CheckPoint::SetCollisionSize(_vec3 size)
{
    auto collision = GetComponent<CollisionComponent>();
    if (collision)
    {
        collision->SetSize(size);
    }
}

void CheckPoint::SetTriggerPosition(_vec3 pos)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(pos);
}

void CheckPoint::OnCollisionEnter(Object* other)
{
    auto player = dynamic_cast<Player*>(other);
    if (!player) return;
    player->SetSpawnPointFromTrigger(scene, num);
    SetDead();
}

void CheckPoint::Free()
{
    Object::Free();
}
