#include "EnginePCH.h"
#include "CollisionSystem.h"

//component
#include "CollisionComponent.h"

CollisionSystem::CollisionSystem()
{
}

CollisionSystem::~CollisionSystem()
{
}

CollisionSystem* CollisionSystem::Create()
{
	CollisionSystem* Instance = new CollisionSystem;

	if (FAILED(Instance->Ready_CollisionSystem()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT CollisionSystem::Ready_CollisionSystem()
{

	return S_OK;
}

void CollisionSystem::Update()
{
}

void CollisionSystem::RegisterCollision(CollisionComponent* collision)
{
	Collisions.push_back(collision);
}

HitInfo CollisionSystem::Raycast(Ray ray)
{
	HitInfo hit;

	for (const auto& collision : Collisions)
		collision->RayIntersectAABB(ray, hit);

	return hit;
}

void CollisionSystem::Free()
{

}
