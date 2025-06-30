#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL CollisionComponent :
    public ObjectComponent
{
private:
    CollisionComponent(Object* owner);
    virtual ~CollisionComponent();

public:
    static CollisionComponent* Create(Object* owner);

    void SetSize(_vec3 size);
    void SetSize(_float x, _float y, _float z);

    void SetOffset(_vec3 offset);
    void SetOffset(_float x, _float y, _float z);
    
private:
    void Free()override;

    _vec3 Size{ 1.f,1.f,1.f };
    _vec3 Offset{ 0.f,0.f,0.f };
    LPD3DXMESH BoundingBox = nullptr;
};

END