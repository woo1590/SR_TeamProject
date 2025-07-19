#pragma once
#include "Collider.h"

BEGIN(Engine)

class TransformComponent;
class ENGINE_DLL AABBCollider :
    public Collider
{
private:
    AABBCollider(CollisionComponent* owner);
    virtual ~AABBCollider();

public:
    static AABBCollider* Create(CollisionComponent* owner);
    void Update()override;
    AABB GetWorldAABB() override;
    bool CheckCollision(Collider* other) override;
    bool CollisionAABB(AABBCollider* other) override;
    bool CollisionOBB(OBBCollider* other) override;
    
    void SetSize(_vec3 size)override;
    _vec3 GetSize()const { return localAABB.half * 2.f; }

    void SetOffset(_vec3 offset)override;
    _vec3 GetOffset()const { return localAABB.center; }

private:
    void Free()override;

    AABB localAABB;
    AABB worldAABB;
};

END