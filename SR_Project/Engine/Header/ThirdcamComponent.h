#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class TransformComponent;
class ENGINE_DLL ThirdcamComponent : public ObjectComponent
{
private:
    ThirdcamComponent(Object* owner);
    virtual ~ThirdcamComponent();

public:
    static ThirdcamComponent* Create(Object* owner);
    HRESULT Ready_Component()override;
    void Update(_float dt)override;

    void SetFollowTarget(TransformComponent* target);
    void SetFollowTarget(Object* target);

    void SetOffset(_vec3 offset);
    void SetOffset(_float x, _float y, _float z);

private:
    void Free()override;

    TransformComponent* Target = nullptr;
    TransformComponent* FollowTarget = nullptr;

    _vec3 Offset{ 0.f,20.f,-20.f };
    _vec3 Direction{ 0.f,0.f,0.f };
};

END