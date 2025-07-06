#pragma once
#include "Base.h"

BEGIN(Engine)

class CollisionComponent;
class ENGINE_DLL StaticGrid :
    public Base
{
private:
    StaticGrid();
    virtual ~StaticGrid();

public:
    static StaticGrid* Create();
    HRESULT Ready_StaticGrid();

    int WorldToCell(_float v);
    void InsertBlock(int cx, int cy, int cz, CollisionComponent* comp);
    CollisionComponent* QueryCell(int cx, int cy, int cz);

private:
    void Free()override;

    static constexpr float CELL_SIZE = 2.f;
    UINT64 HashCell(int cx, int cy, int cz);
    std::unordered_map<UINT64, CollisionComponent*> Cells;
};

END