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
	CurrCollision.clear();

	//Check Collision
	for (_uint i = 0; i < Collisions.size(); ++i)
	{
		CollisionComponent* a = Collisions[i];

		for (_uint j = i+1; j < Collisions.size(); ++j)
		{
			CollisionComponent* b = Collisions[j];

			if (a->CanCollision(b) && b->CanCollision(a))
			{
				if (a->CheckAABBCollision(b))
				{
					CurrCollision.push_back(std::pair<CollisionComponent*, CollisionComponent*>(a, b));
				}
			}
		}
	}

	//Collision Enter
	for (const auto& currPair : CurrCollision)
	{
		currPair.first->OnCollisionEnter(currPair.second);
		currPair.second->OnCollisionEnter(currPair.first);
	}

	//Collision Stay
	for (const auto& prevPair : PrevCollision)
	{
		for (const auto& currPair : CurrCollision)
		{
			auto it = std::find(PrevCollision.begin(), PrevCollision.end(), currPair);
			if (it != PrevCollision.end())
			{
				currPair.first->OnCollisionStay(currPair.second);
				currPair.second->OnCollisionStay(currPair.first);
			}
		}
	}

	//Collision Exit
	for (const auto& prevPair : PrevCollision)
	{
		for (const auto& currPair : CurrCollision)
		{
			auto it = std::find(PrevCollision.begin(), PrevCollision.end(), currPair);
			if (it != PrevCollision.end())
			{
				currPair.first->OnCollisionExit(currPair.second);
				currPair.second->OnCollisionExit(currPair.first);
			}
		}
	}

	PrevCollision.swap(CurrCollision);
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
