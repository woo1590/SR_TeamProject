#pragma once
#include "Effect.h"
class TikkleEffect :
    public Effect
{
    TikkleEffect(ObjectManager* owner, ObjectType objType, Object* _onPos);
    virtual ~TikkleEffect();

public:
    static TikkleEffect* Create(ObjectManager* owner, ObjectType objType, Object* _onPos);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _float Scale = 0.f;
    _vec3 startPos = { 0.f,0.f,0.f };
    _vec3 destinationPos = { 0.f,0.f,0.f };
    _vec3 moveDir = { 0.f,0.f,0.f };
};

