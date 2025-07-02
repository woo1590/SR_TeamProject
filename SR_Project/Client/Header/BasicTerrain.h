#pragma once
#include "Object.h"

class BasicTerrain :
    public Object
{
    friend class Object;
private:
    BasicTerrain(ObjectManager* owner, ObjectType objType);
    virtual ~BasicTerrain();

public:
    static BasicTerrain* Create(ObjectManager* owner, ObjectType objType);

    HRESULT Ready_Object();
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

