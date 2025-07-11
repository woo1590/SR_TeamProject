#include "pch.h"
#include "Boss.h"

Boss::Boss(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Boss::~Boss()
{

}

HRESULT Boss::Ready_Object(ObjectManager* owner, ObjectType objType)
{
	Monster::Ready_Object(owner, objType, MonsterType::Boss);
	
	return S_OK;
}

void Boss::Update(_float dt)
{
	Monster::Update(dt);
}

void Boss::Late_Update(_float dt)
{
	Monster::Late_Update(dt);
}

void Boss::MoveTo(_vec3* dir, _float dt)
{
}

void Boss::RotateTo(_vec3* dir, float dt)
{
}

void Boss::Attack(Object* target)
{
}

void Boss::Die()
{
}

void Boss::Hit(_vec3 dir, _float power)
{
}

void Boss::InitAnimation()
{
}

void Boss::PlayAnimation(_float dt)
{
}

void Boss::PlayIdle(_float dt)
{
}

void Boss::PlayWalk(_float dt)
{
}

void Boss::PlayAttack(_float dt)
{
}

void Boss::PlayDie(_float dt)
{
}

void Boss::PlayHit(_float dt)
{
}

void Boss::OnCollisionStay(Object* other)
{
}

void Boss::Free()
{
}
