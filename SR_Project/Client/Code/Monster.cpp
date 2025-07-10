#include "pch.h"
#include "Scene.h"
#include "Monster.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "InfoComponent.h"
#include "InfoDetector.h"
#include "TransformComponent.h"

Monster::Monster(ObjectManager* owner, ObjectType objType)
	:BaseCharacter(owner, objType)
{
}

Monster::~Monster()
{
}

HRESULT Monster::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);

    auto  statcomponent = AddComponent<InfoComponent<EnemyInfo>>();

    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetLayer(CollisionComponent::LAYER_ENEMY);
    collision->SetMask(CollisionComponent::LAYER_PLAYER | CollisionComponent::LAYER_DEFAULT);
    collision->SetCollisionStay([this](Object* other) {this->OnCollisionStay(other); });
    
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);

    IsHit = nullptr;

    return S_OK;
}

void Monster::Update(_float dt)
{
    BaseCharacter::Update(dt);
}

void Monster::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Monster::MoveTo(_vec3* dir, _float dt)
{
}

void Monster::RotateTo(_vec3* dir, float dt)
{
}

void Monster::Attack(Object* target)
{
}

void Monster::Die()
{

}

void Monster::Hit(_vec3 dir, _float power)
{
}

_float Monster::GetHp()
{
    auto  statcomponent = GetComponent<InfoComponent<EnemyInfo>>();
    return statcomponent->GetInfo().curHp;
}

void Monster::BackStep(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto Stat = GetComponent<InfoComponent<EnemyInfo>>();

    if (State != MonsterState::Walk) State = MonsterState::Walk;
    D3DXVec3Normalize(dir, dir);
    Transform->Translate(*dir * dt * Stat->GetInfo().speed);
}

void Monster::SetHit(_bool Hit)
{
    if (IsHit != nullptr) *IsHit = Hit;
}

_bool Monster::GetHit()
{
    if (IsHit != nullptr)
        return *IsHit;
    else
        return false;
}

void Monster::InitAnimation()
{
   
}

void Monster::PlayAnimation(_float dt)
{
}

void Monster::PlayIdle(_float dt)
{
    //idle animation
}

void Monster::PlayWalk(_float dt)
{
}

void Monster::PlayAttack(_float dt)
{
}

void Monster::PlayDie(_float dt)
{
}

void Monster::PlayHit(_float dt)
{
}

void Monster::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    //if (objType == ObjectType::StaticBlock)
    //    collision->ResolveAABBColiision(other);
}

void Monster::Free()
{
    Safe_Delete(Distance);
    Safe_Delete(IsAttack);
    Safe_Delete(IsHit);
    BaseCharacter::Free();
}