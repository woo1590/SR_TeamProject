#pragma once
#include "DeadEffect.h"

class EnderDead :
    public DeadEffect
{
private:
    EnderDead(ObjectManager* owner, ObjectType objType);
    virtual ~EnderDead();

public:
    static EnderDead* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

