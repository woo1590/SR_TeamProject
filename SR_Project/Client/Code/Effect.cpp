#include "pch.h"
#include "Effect.h"

//component
#include "TransformComponent.h"
#include "SpriteRenderer.h"

Effect::Effect(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

Effect::~Effect()
{
}

void Effect::Update(_float dt)
{
	Object::Update(dt);

	if (ObjType == ObjectType::ParticleEffect)
	{
		if (deadTime)
			timer += dt;
	}
	else
	{
		auto sprite = GetComponent<SpriteRenderer>();
		if (sprite->IsSpriteEnd() && isDeadBySpriteEnd)
		{
			SetDead();
		}
	}
}

void Effect::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}
