#pragma once
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL OBBCollider :
    public Collider
{
private:
    OBBCollider(CollisionComponent* owner);
    virtual ~OBBCollider();

public:
    static OBBCollider * Create(CollisionComponent* owner);
    AABB GetWorldAABB() override;
    bool CheckCollision(Collider* other) override;
    bool CollisionAABB(AABBCollider* other) override;
    bool CollisionOBB(OBBCollider* other) override;

    void SetSize(_vec3 size) override;
    _vec3 GetSize()const { return localOBB.half * 2.f; }

    void SetOffset(_vec3 offset) override;
    _vec3 GetOffset()const { return localOBB.center; }

    OBB GetWorldOBB();

    void Render()override{}

private:
    void Free()override;

    OBB localOBB;
};

END