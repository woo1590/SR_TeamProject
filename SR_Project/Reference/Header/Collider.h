#pragma once
#include "Base.h"

BEGIN(Engine)

class AABBCollider;
class OBBCollider;
class TransformComponent;
class CollisionComponent;

class ENGINE_DLL Collider :
    public Base
{
protected:
    Collider(CollisionComponent* owner):owner(owner){}
    virtual ~Collider() = default;

public:
    virtual AABB GetWorldAABB()PURE;
    virtual bool CheckCollision(Collider* other)PURE;
    virtual bool CollisionAABB(AABBCollider* other)PURE;
    virtual bool CollisionOBB(OBBCollider* other)PURE;

    virtual void SetSize(_vec3 size)PURE;
    virtual _vec3 GetSize()const PURE;

    virtual void SetOffset(_vec3 offset)PURE;
    virtual _vec3 GetOffset()const PURE;

    virtual void Render()PURE;
protected:
    void Free()override {}

    CollisionComponent* owner = nullptr;

    LPD3DXMESH debugBox = nullptr;
};

END