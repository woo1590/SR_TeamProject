#pragma once
#include "ObjectComponent.h"
#include "CollisionSystem.h"

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
    void Late_Update(_float dt)override;

    void SetOffset(_vec3 offset);
    void SetOffset(_float x, _float y, _float z);

    void SetBoudingBox(BoundingBoxType bbType);
    void SetSize(_vec3 size);

    _vec3 GetLocalMin()const;
    _vec3 GetLocalMax()const;

    /*----------------Collision-----------------*/
    _bool RayIntersectAABB(Ray ray, HitInfo& hit);

    _bool CheckAABBCollision(CollisionComponent* other);
    void ResolveAABBColiision(Object* other);

    void Render();
private:

    void Free()override;

    _vec3 Offset{ 0.f,0.f,0.f };

    LPD3DXMESH BoundingBox = nullptr;   //디버그용 메쉬 
    BoundingBoxType BBType = BoundingBoxType::Box;

    _vec3 LocalMin{ -1.f,-1.f,-1.f };
    _vec3 LocalMax{ 1.f,1.f,1.f };
    /*----------------------*/
};

END