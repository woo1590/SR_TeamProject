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
	GetScene()->GetCollisionSystem()->RegisterCollision(collision);
	collision->SetLayer(CollisionComponent::LAYER_DEFAULT);
	collision->SetMask(CollisionComponent::LAYER_PLAYER | CollisionComponent::LAYER_ENEMY);

	auto physics = AddComponent<PhysicsComponent>();

	return S_OK;
}

void CollisionBlock::Free()
{
	Object::Free();
}
