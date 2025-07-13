#pragma once
#include "Effect.h"

class HitEffect :
    public Effect
{
private:
    HitEffect(ObjectManager* owner, ObjectType objType);
    virtual ~HitEffect();

public:
    static HitEffect* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object();
    void Update(_float dt);
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

