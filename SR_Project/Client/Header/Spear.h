#pragma once
#include "Item.h"
class Spear :
    public Item
{
    Spear(ObjectManager* owner, ObjectType objType);
    virtual ~Spear();
    void Free() override;
public:
    static Spear* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetCollisionEnter(Object* other);

    void PlayerSpearInfo();
};

