#pragma once
#include "Effect.h"
class RollEffect :
    public Effect
{
    RollEffect(ObjectManager* owner, ObjectType objType, Object* _onPos, _float _moveAngle);
    virtual ~RollEffect();

public:
    static RollEffect* Create(ObjectManager* owner, ObjectType objType, Object* _onPos, _float _moveAngle);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _float Scale = 0.f;
    _float moveAngle = 0.f;
    _vec3 startPos = { 0.f,0.f,0.f };
    _vec3 moveVec = { 0.f,0.f,0.f };
    _float moveSpeed = 2.f;
};

