#include "pch.h"
#include "SpriteEffect.h"

//component
#include "TransformComponent.h"
#include "SpriteRenderer.h"

SpriteEffect::SpriteEffect(ObjectManager* owner, ObjectType objType)
	:Effect(owner,objType)
{
}

SpriteEffect::~SpriteEffect()
{
}

SpriteEffect* SpriteEffect::Create(ObjectManager* owner, ObjectType objType)
{
	SpriteEffect* Instance = new SpriteEffect(owner, objType);
	
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT SpriteEffect::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	
	return S_OK;
}
