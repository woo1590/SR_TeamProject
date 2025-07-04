#pragma once
#include "Item.h"
class Sword :
    public Item
{
private:
    Sword(ObjectManager* owner, ObjectType objType);
    virtual ~Sword();

public:
    static Sword* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
private:
    void Free() override;
};

