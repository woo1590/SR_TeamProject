#pragma once
#include "Effect.h"
class ChargeDownEffect :
    public Effect
{
    ChargeDownEffect(ObjectManager* owner, ObjectType objType, Object* _onPos);
    virtual ~ChargeDownEffect();

public:
    static ChargeDownEffect* Create(ObjectManager* owner, ObjectType objType, Object* _onPos);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;
    _float Scale = 0.f;
};

