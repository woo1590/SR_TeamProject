#pragma once
#include "Item.h"
class Bow :
    public Item
{
public:
    static Bow* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    Bow(ObjectManager* owner, ObjectType objType);
    virtual ~Bow();
    void Free() override;
};

