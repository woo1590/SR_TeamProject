#pragma once
#include "Effect.h"
class ChargeOnEffect :
    public Effect
{
    ChargeOnEffect(ObjectManager* owner, ObjectType objType, Object* _onPos);
    virtual ~ChargeOnEffect();

public:
    static ChargeOnEffect* Create(ObjectManager* owner, ObjectType objType, Object* _onPos);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _float Scale = 0.f;
};

