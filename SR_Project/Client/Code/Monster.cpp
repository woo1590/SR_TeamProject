#include "pch.h"
#include "Monster.h"
#include "IsTargetInAttackRange.h"
#include "Chase.h"
#include "Attack.h"
#include "SequenceNode.h"
#include "SelectorNode.h"
#include "ObjectManager.h"
#include "BehaviorTree.h"
#include "BlackBoard.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "AIController.h"
#include "IsAlive.h"
#include "Die.h"

Monster::Monster(ObjectManager* owner, ObjectType objType)
	:BaseCharacter(owner, objType)
{
}

Monster::~Monster()
{
}

HRESULT Monster::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    return S_OK;
}

void Monster::Update(_float dt)
{
    BaseCharacter::Update(dt);
    PlayAnimation(dt);
}

void Monster::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Monster::MoveTo(_vec3* dir, _float dt)
{
}

void Monster::Attack(Object* target)
{
}

void Monster::Die()
{

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

void Monster::Free()
{
    BaseCharacter::Free();
}
