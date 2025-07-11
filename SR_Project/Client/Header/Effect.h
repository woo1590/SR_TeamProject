#pragma once
#include "Object.h"

class Effect :
    public Object
{
private:
    Effect(ObjectManager* owner, ObjectType objType);
    virtual ~Effect();

public:
    virtual HRESULT Ready_Object()override {};
    virtual void Update(_float dt)override {};
    virtual void Late_Update(_float dt)override {};
    
private:
    virtual void Free()override;
};

