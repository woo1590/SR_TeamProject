#pragma once
#include "Item.h"
class Sword :
    public Item
{
public:
    static Sword* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    Sword(ObjectManager* owner, ObjectType objType);
    virtual ~Sword();
    void Free() override;

    void SetCollisionEnter(Object* other);
};

