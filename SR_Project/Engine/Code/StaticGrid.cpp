#include "EnginePCH.h"
#include "StaticGrid.h"
#include "CollisionComponent.h"

StaticGrid::StaticGrid()
{
}

StaticGrid::~StaticGrid()
{
}

StaticGrid* StaticGrid::Create()
{
	StaticGrid* Instance = new StaticGrid;

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

void StaticGrid::InsertBlock(int cx, int cy, int cz, CollisionComponent* comp)
{
	UINT64 key = HashCell(cx, cy, cz);

	Cells[key] = comp;
	comp->AddRef();
}

CollisionComponent* StaticGrid::QueryCell(int cx, int cy, int cz)
{
	UINT64 key = HashCell(cx, cy, cz);

	auto iter = Cells.find(key);

	return (iter != Cells.end()) ? iter->second : nullptr;
}

void StaticGrid::Free()
{
	std::for_each(Cells.begin(), Cells.end(), [](auto& pair)
		{
			Safe_Release(pair.second);
		});
}

UINT64 StaticGrid::HashCell(int cx, int cy, int cz)
{
	const UINT64 MASK = (1ull << 21) - 1;

	return (UINT64)(cx & MASK)
		 | (UINT64)((cy & MASK)<<21)
		 | (UINT64)((cz & MASK)<<42);
}
