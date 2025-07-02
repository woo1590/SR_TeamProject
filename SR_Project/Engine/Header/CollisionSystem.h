#pragma once
#include "Base.h"

BEGIN(Engine)

class CollisionComponent;
class ENGINE_DLL CollisionSystem :
    public Base
{
public:
    typedef struct tagHitInfo
    {
        _bool Hit
    }HitInfo;

private:
    CollisionSystem();
    virtual ~CollisionSystem();

public:
    static CollisionSystem* Create();
    HRESULT Ready_CollisionSystem();
    void Update();
    void RegisterCollision(CollisionComponent* collision);

    void Raycast(Ray ray);   //���콺 ��ŷ ���� ó��

private:
    void Free()override;
    std::vector<CollisionComponent*> Collisions;
    Scene* owner = nullptr;
};

END