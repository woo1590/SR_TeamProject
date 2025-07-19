#include "EnginePCH.h"
#include "CollisionBlock.h"
#include "Scene.h"

#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

CollisionBlock::CollisionBlock(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

CollisionBlock::~CollisionBlock()
{
}

CollisionBlock* CollisionBlock::Create(ObjectManager* owner, ObjectType objType)
{
	CollisionBlock* Instance = new CollisionBlock(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);

	}

	return Instance;
}

HRESULT CollisionBlock::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();

	auto collision = AddComponent<CollisionComponent>();
	collision->SetLayer(LAYER_DEFAULT);
	collision->SetMask(LAYER_PLAYER | LAYER_ENEMY | LAYER_PROJECTILE | LAYER_TRRIGER | LAYER_NPC | LAYER_DEFAULT);
	collision->SetDebugMode(false);

	auto physics = AddComponent<PhysicsComponent>();

	return S_OK;
}

void CollisionBlock::Free()
{
	Object::Free();
}
