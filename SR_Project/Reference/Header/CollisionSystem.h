#pragma once
#include "Base.h"

BEGIN(Engine)

class CollisionComponent;
typedef struct tagHitInfo
{
    CollisionComponent* Component = nullptr;
    _float              Distance = FLT_MAX;
    _vec3               Position{ 0.f,0.f,0.f };

}HitInfo;

class ENGINE_DLL CollisionSystem :
    public Base
{
private:
    CollisionSystem();
    virtual ~CollisionSystem();

public:
    static CollisionSystem* Create();
    HRESULT Ready_CollisionSystem();
    void Update();
    void RegisterCollision(CollisionComponent* collision);

    HitInfo Raycast(Ray ray);   //���콺 ��ŷ ���� ó��

private:
    void Free()override;
    std::list<CollisionComponent*> Collisions;
};

END