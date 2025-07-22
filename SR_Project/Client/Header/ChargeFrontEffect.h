#pragma once
#include "Effect.h"
class ChargeFrontEffect :
    public Effect
{
    ChargeFrontEffect(ObjectManager* owner, ObjectType objType, Object* _onPos);
    virtual ~ChargeFrontEffect();

public:
    static ChargeFrontEffect* Create(ObjectManager* owner, ObjectType objType, Object* _onPos);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _float Scale = 0.f;
    _float tikkleTimer = 0.f;
    const _float tikkleTerm = 0.005f;

    _float soundTimer = 0.f;
    const _float soundTerm = 1.1f;
};

