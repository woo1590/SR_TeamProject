#pragma once
#include "Base.h"

BEGIN(Engine)

class Scene;
class CollisionComponent;
class ENGINE_DLL StaticGrid :
    public Base
{
private:
    StaticGrid(Scene* owner);
    virtual ~StaticGrid();

public:
    static StaticGrid* Create(Scene* owner);
    HRESULT Ready_StaticGrid();

    int WorldToCell(_float v);
    CollisionComponent* QueryCell(int cx, int cy, int cz);
    void InsertBlock();

private:
    void Free()override;

    UINT64 HashCell(int cx, int cy, int cz);

    Scene* owner = nullptr;
    static constexpr float CELL_SIZE = 2.f;
    std::unordered_map<UINT64, CollisionComponent*> Cells;
};

END