#pragma once
#include "Object.h"
class Item :
    public Object
{
public:
    enum class ItemType {
        ITEM_SWORD,
        ITEM_BOW
    };
public:
    static Item* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    Item(ObjectManager* owner, ObjectType objType);
    virtual ~Item();
    void Free() override;
};

