#pragma once
#include "Object.h"

class SkyBox :
    public Object
{
    friend class Object;
private:
    SkyBox(ObjectManager* owner, ObjectType objType);
    virtual ~SkyBox();

public:
    static SkyBox* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

