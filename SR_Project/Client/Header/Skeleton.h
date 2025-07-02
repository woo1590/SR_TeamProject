#pragma once
#include "Monster.h"
class Skeleton :
    public Monster
{
protected:
    Skeleton(ObjectManager* owner, ObjectType objType);
    virtual ~Skeleton();

public:
    static Skeleton* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    void Free() override;
};

