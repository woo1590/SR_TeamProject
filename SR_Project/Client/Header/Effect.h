#pragma once
#include "Object.h"

class Effect :
    public Object
{
protected:
    Effect(ObjectManager* owner, ObjectType objType);
    virtual ~Effect();

public:
    virtual HRESULT Ready_Object()override { return S_OK; };
    virtual void Update(_float dt)override {};
    virtual void Late_Update(_float dt)override {};
    
protected:
    virtual void Free()override{}
};

