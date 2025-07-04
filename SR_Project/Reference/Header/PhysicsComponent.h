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
    HRESULT Ready_Component()override;
    void Update(_float dt)override;

    void SetVelocity(_vec3 velocity);
    void SetVelocity(_float x, _float y, _float z);
    _vec3 GetVelocity()const;

    void SetMass(_float mass) { Mass = mass; }
    _float GetMass()const { return Mass; }
    _float GetInvMass()const { return Mass ? 1.f / Mass : 0.f; }

    void SetGround(_bool ground);
    _bool IsGrounded()const;

    _vec3 Position; //Transform
    _vec3 PredPosition; //Next Frame Position
private:
    void Free()override;

    _bool Is_Ground = false;
    _vec3 Velocity{ 0.f,0.f,0.f };
    _float Mass = 0.f;
};

END