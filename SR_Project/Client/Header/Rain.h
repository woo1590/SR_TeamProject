#pragma once
#include "Effect.h"

class Rain :
    public Effect
{
private:
    Rain(ObjectManager* owner, ObjectType objType);
    virtual ~Rain();

public:
    static Rain* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;

};

