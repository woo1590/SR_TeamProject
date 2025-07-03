#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL PhysicsComponent :
    public ObjectComponent
{
private:
    PhysicsComponent(Object* owner);
    virtual ~PhysicsComponent();

public:
    static PhysicsComponent* Create(Object* owner);
    void Update(_float dt)override;

    void SetVelocity(_vec3 velocity);
    void SetVelocity(_float x, _float y, _float z);

    _vec3 GetVelocity()const;

    void SetGround(_bool ground);
    _bool IsGrounded()const;
private:
    void Free()override;

    _bool Is_Ground = false;
    _vec3 Velocity{ 0.f,0.f,0.f };
    static constexpr _float GRAVITY = 30.f;
};

END