#pragma once
#include "Object.h"
class Projectile : public Object
{
public:
    static Projectile* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    Projectile(ObjectManager* owner, ObjectType objType);
    virtual ~Projectile();
    void Free() override;
};

