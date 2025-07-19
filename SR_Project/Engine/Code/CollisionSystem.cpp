#include "EnginePCH.h"
#include "CollisionSystem.h"

//component
#include "CollisionComponent.h"

CollisionSystem::CollisionSystem(Scene* owner)
	:owner(owner)
{
}

CollisionSystem::~CollisionSystem()
{
}

CollisionSystem* CollisionSystem::Create(Scene* owner)
{
	CollisionSystem* Instance = new CollisionSystem(owner);

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

void CollisionSystem::Late_Update()
{

}

void CollisionSystem::RegisterCollision(CollisionComponent* collision)
{
	Collisions.push_back(collision);
}

void CollisionSystem::UnRegisterCollision(CollisionComponent* collision)
{
	for (auto iter = Collisions.begin(); iter != Collisions.end(); ++iter)
	{
		if ((*iter) == collision)
		{
			Collisions.erase(iter);
			break;
		}
	}
}

HitInfo CollisionSystem::Raycast(Ray ray)
{
	HitInfo res;

	for (const auto& collision : Collisions)
	{
		collision->RayIntersectAABB(ray, res);
	}

	return res;
}

void CollisionSystem::ThreadWork(Ray ray, int startIdx, int endIdx, HitInfo& res)
{
	
}

void CollisionSystem::Free()
{

}
