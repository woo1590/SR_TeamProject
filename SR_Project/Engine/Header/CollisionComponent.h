#pragma once
#include "ObjectComponent.h"
#include "CollisionSystem.h"

BEGIN(Engine)

class ENGINE_DLL CollisionComponent :
    public ObjectComponent
{
public:
    enum Layer : _ulong
    {
        LAYER_DEFAULT       = 1 << 0,
        LAYER_PLAYER        = 1 << 1,
        LAYER_ENEMY         = 1 << 2,
        LAYER_PROJECTILE    = 1 << 3,
    };

private:
    CollisionComponent(Object* owner);
    virtual ~CollisionComponent();

public:
    static CollisionComponent* Create(Object* owner);
    HRESULT Ready_Component()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void SetOffset(_vec3 offset);
    void SetOffset(_float x, _float y, _float z);
    _vec3 GetOffset()const { return Offset; }

    void SetBoudingBox(BoundingBoxType bbType);
    void SetSize(_vec3 size);

    void SetLayer(CollisionComponent::Layer layer);
    void SetMask(_ulong mask);

    _ulong GetLayer()const { return Collision_Layer; }
    _ulong GetMask()const { return Collision_Mask; }

    _vec3 GetLocalMin()const;
    _vec3 GetLocalMax()const;
    void GetWorldAABB(_vec3* worldMin, _vec3* worldMax);
    void GetWorldAABB(_vec3* worldMin, _vec3* worldMax, _vec3 pos);
    void GetWorldX(_float* minX, _float* maxX);

    /*----------------Collision-----------------*/
    _bool RayIntersectAABB(Ray ray, HitInfo& hit);

    _bool CanCollision(CollisionComponent* other);

    _bool CheckAABBCollision(CollisionComponent* other);
    void ResolveAABBColiision(Object* other);

    void OnCollisionEnter(CollisionComponent* other);
    void OnCollisionStay(CollisionComponent* other);
    void OnCollisionExit(CollisionComponent* other);

    void SetCollisionEnter(std::function<void(Object*)> enter) { onEnter = enter; }
    void SetCollisionStay(std::function<void(Object*)> stay) { onStay = stay; }
    void SetCollisionExit(std::function<void(Object*)> exit) { onExit = exit; }

    /*-------------------------------------------*/
    void Render();  //Debug
private:

    void Free()override;

    _vec3 Offset{ 0.f,0.f,0.f };

    LPD3DXMESH BoundingBox = nullptr;   //����׿� �޽� 
    BoundingBoxType BBType = BoundingBoxType::Box;

    _vec3 LocalMin{ -1.f,-1.f,-1.f };
    _vec3 LocalMax{ 1.f,1.f,1.f };

    /*----------------------*/
    
    Layer Collision_Layer;
    _ulong Collision_Mask;

    std::function<void(Object* other)> onEnter;
    std::function<void(Object* other)> onStay;
    std::function<void(Object* other)> onExit;
};

END