#pragma once
#include "Base.h"
#include "CollisionSystem.h"

BEGIN(Engine)

class Scene;
class CollisionComponent;
class Object;
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
    HitInfo RayCast(Ray ray);
    CollisionComponent* QueryCell(int cx, int cy, int cz);
    void InsertBlock();
    void InsertBlock(Object* block);

    _vec3 GetRandomGrid(_vec3 originPos, _float range);
    StaticBlockType GetBlockType(_vec3 pos);
private:
    void Free()override;

    UINT64 HashCell(int cx, int cy, int cz);

    Scene* owner = nullptr;
    static constexpr float CELL_SIZE = 2.f;
    std::unordered_map<UINT64, CollisionComponent*> Cells;
};

END