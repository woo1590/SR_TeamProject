#include "EnginePCH.h"
#include "StaticGrid.h"
#include "Scene.h"
#include "Object.h"
#include "ObjectManager.h"
#include "CollisionBlock.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

StaticGrid::StaticGrid(Scene* owner)
	:owner(owner)
{
}

StaticGrid::~StaticGrid()
{
}

StaticGrid* StaticGrid::Create(Scene* owner)
{
	StaticGrid* Instance = new StaticGrid(owner);

	if (FAILED(Instance->Ready_StaticGrid()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT StaticGrid::Ready_StaticGrid()
{
	Cells.clear();

	return S_OK;
}

int StaticGrid::WorldToCell(_float v)
{
	return static_cast<int>(std::floorf(v / CELL_SIZE));
}

HitInfo StaticGrid::RayCast(Ray ray)
{
    HitInfo hit; 

    int cx = WorldToCell(ray.Origin.x);
    int cy = WorldToCell(ray.Origin.y);
    int cz = WorldToCell(ray.Origin.z);

    int stepX = (ray.Direction.x >= 0) ? 1 : -1;
    int stepY = (ray.Direction.y >= 0) ? 1 : -1;
    int stepZ = (ray.Direction.z >= 0) ? 1 : -1;

    float deltaX = (ray.Direction.x == 0) ? FLT_MAX : std::abs(CELL_SIZE / ray.Direction.x);
    float deltaY = (ray.Direction.y == 0) ? FLT_MAX : std::abs(CELL_SIZE / ray.Direction.y);
    float deltaZ = (ray.Direction.z == 0) ? FLT_MAX : std::abs(CELL_SIZE / ray.Direction.z);

    float nextBoundaryX = (stepX > 0) ? (cx + 1) * CELL_SIZE : cx * CELL_SIZE;
    float nextBoundaryY = (stepY > 0) ? (cy + 1) * CELL_SIZE : cy * CELL_SIZE;
    float nextBoundaryZ = (stepZ > 0) ? (cz + 1) * CELL_SIZE : cz * CELL_SIZE;

    float maxX = (ray.Direction.x == 0) ? FLT_MAX : (nextBoundaryX - ray.Origin.x) / ray.Direction.x;
    float maxY = (ray.Direction.y == 0) ? FLT_MAX : (nextBoundaryY - ray.Origin.y) / ray.Direction.y;
    float maxZ = (ray.Direction.z == 0) ? FLT_MAX : (nextBoundaryZ - ray.Origin.z) / ray.Direction.z;

    for (int i = 0; i < 1000; ++i)
    {
        CollisionComponent* block = QueryCell(cx, cy, cz);
        if (block)
        {
            block->RayIntersectAABB(ray, hit);

            float min = (std::min)(maxX, (std::min)(maxY, maxZ));

            if (hit.IsHit && hit.Distance < min)
            {
                return hit;
            }
        }

        if (maxX < maxY)
        {
            if (maxX < maxZ)
            {
                cx += stepX;
                maxX += deltaX;
            }
            else
            {
                cz += stepZ;
                maxZ += deltaZ;
            }
        }
        else
        {
            if (maxY < maxZ)
            {
                cy += stepY;
                maxY += deltaY;
            }
            else
            {
                cz += stepZ;
                maxZ += deltaZ;
            }
        }
    }

    return hit;

}

void StaticGrid::InsertBlock()
{
	Cells.clear();

	auto collisionBlocks = owner->GetObjectManager()->GetObjectList(ObjectType::CollisionBlock);
	
	for (const auto& block : collisionBlocks)
	{
		_vec3 pos = block->GetComponent<TransformComponent>()->GetPosition();
		auto collision = block->GetComponent<CollisionComponent>();

		int cx = WorldToCell(pos.x);
		int cy = WorldToCell(pos.y);
		int cz = WorldToCell(pos.z);

		UINT64 key = HashCell(cx, cy, cz);

		Cells[key] = collision;
	}
}

_vec3 StaticGrid::GetRandomGrid(_vec3 originPos, _float range)
{
    int minX = WorldToCell(originPos.x - range);
    int minY = WorldToCell(originPos.y - range);
    int minZ = WorldToCell(originPos.z - range);

    int maxX = WorldToCell(originPos.x + range);
    int maxY = WorldToCell(originPos.y + range);
    int maxZ = WorldToCell(originPos.z + range);

    //for(int x=minX; x<maxX; ++x)
    //    for(int )
    return _vec3();
}

StaticBlockType StaticGrid::GetBlockType(_vec3 pos)
{
    int cx = WorldToCell(pos.x);
    int cy = WorldToCell(pos.y);
    int cz = WorldToCell(pos.z);
    int key = HashCell(cx, cy, cz);

    return static_cast<CollisionBlock*>(Cells[key]->GetOwner())->GetType();
}

CollisionComponent* StaticGrid::QueryCell(int cx, int cy, int cz)
{
	UINT64 key = HashCell(cx, cy, cz);

	auto iter = Cells.find(key);

	return (iter != Cells.end()) ? iter->second : nullptr;
}

void StaticGrid::Free()
{

}

UINT64 StaticGrid::HashCell(int cx, int cy, int cz)
{
	const UINT64 MASK = (1ull << 21) - 1;

	return (UINT64)(cx & MASK)
		 | (UINT64)((cy & MASK)<<21)
		 | (UINT64)((cz & MASK)<<42);
}
