#pragma once
#include "Effect.h"

class DeadEffect :
    public Effect
{
private:
    DeadEffect(ObjectManager* owner, ObjectType objType);
    virtual ~DeadEffect();

public:
    static DeadEffect* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;

    void Update(_float dt)override;

private:
    void Free()override;

    _uint totalFrame = 0;
    _uint currFrame = 0;
};

