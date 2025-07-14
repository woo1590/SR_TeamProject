#pragma once
#include "Effect.h"

class BloodEffect :
    public Effect
{
private:
    BloodEffect(ObjectManager* owner, ObjectType objType);
    virtual ~BloodEffect();

public:
    static BloodEffect* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object();
    void Update(_float dt);
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

