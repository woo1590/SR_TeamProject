#pragma once
#include "Projectile.h"
class Arrow : public Projectile
{
public:
    static Arrow* Create(ObjectManager* owner, ObjectType objType, ObjectType shooterType, _vec3 normalDirection);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType, ObjectType shooterType, _vec3 normalDirection);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    Arrow(ObjectManager* owner, ObjectType objType);
    virtual ~Arrow();
    void Free() override;
    void SetCollisionEnter(Object* other);
};

