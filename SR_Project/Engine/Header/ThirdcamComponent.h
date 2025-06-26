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
    void Update(_float dt)override;

    void SetTarget(TransformComponent* target);

private:
    void Free()override;

    TransformComponent* Target = nullptr;
    _vec3 Direction{ 0.f,1.f,1.f };
    _float Distance = 50.f;
};

END