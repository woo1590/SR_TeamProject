#pragma once
#include "Item.h"
class Crossbow :
    public Item
{
    Crossbow(ObjectManager* owner, ObjectType objType);
    virtual ~Crossbow();
    void Free() override;
public:
    static Crossbow* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
private:
    void PlayerCrossbowInfo();
};

