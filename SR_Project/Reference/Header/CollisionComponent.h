#pragma once
#include "ObjectComponent.h"
#include "CollisionSystem.h"
#include "AABBCollider.h"
#include "OBBCollider.h"

BEGIN(Engine)

class ENGINE_DLL CollisionComponent :
    public ObjectComponent
{
private:
    CollisionComponent(Object* owner);
    virtual ~CollisionComponent();

public:
    static CollisionComponent* Create(Object* owner);
    HRESULT Ready_Component()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    template<typename T>
    T* AddCollider()
    {
        auto c = T::Create(this);
        collider = c;

        return c;
    }

    void SetOffset(_vec3 offset);
    void SetOffset(_float x, _float y, _float z);
    _vec3 GetOffset()const { return Offset; }

    void SetSize(_vec3 size);
    _vec3 GetSize()const;

    void SetLayer(CollisionLayer layer);
    void SetMask(_ulong mask);

    _ulong GetLayer()const { return Collision_Layer; }
    _ulong GetMask()const { return Collision_Mask; }

    Collider* GetCollider()const { return collider; }
    /*----------------Collision-----------------*/
    _bool RayIntersectAABB(Ray ray, HitInfo& hit);

    _bool CanCollision(CollisionComponent* other);

    _bool CheckCollision(CollisionComponent* other);
    void ResolveAABBColiision(Object* other);

    void OnCollisionEnter(CollisionComponent* other);
    void OnCollisionStay(CollisionComponent* other);
    void OnCollisionExit(CollisionComponent* other);

    void SetCollisionEnter(std::function<void(Object*)> enter) { onEnter = enter; }
    void SetCollisionStay(std::function<void(Object*)> stay) { onStay = stay; }
    void SetCollisionExit(std::function<void(Object*)> exit) { onExit = exit; }

    /*-------------------------------------------*/
    void Render();  //Debug
    void SetDebugMode(bool debug) { DebugMode = debug; }
    _bool IsDebugMode()const { return DebugMode; }
private:

    void Free()override;

    _vec3 Offset{ 0.f,0.f,0.f };

    BoundingBoxType BBType = BoundingBoxType::Box;
    _bool DebugMode = true;

    Collider* collider = nullptr;

    /*----------------------*/
    
    CollisionLayer Collision_Layer;
    _ulong Collision_Mask;

    std::function<void(Object* other)> onEnter;
    std::function<void(Object* other)> onStay;
    std::function<void(Object* other)> onExit;
};

END