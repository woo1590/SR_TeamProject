#pragma once
#include "Effect.h"

class ExplodeEffect :
    public Effect
{
private:
    ExplodeEffect(ObjectManager* owner, ObjectType objType);
    virtual ~ExplodeEffect();

public:
    static ExplodeEffect* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _uint totalFrame = 0;
    _uint currFrame = 0;
};

