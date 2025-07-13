#pragma once
#include "Item.h"
class Tnt :
    public Item
{
    Tnt(ObjectManager* owner, ObjectType objType);
    virtual ~Tnt();
    void Free() override;
public:
    static Tnt* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void TntToPlayer(Object* player);
private:

    void TntInfo();

    void ThrowTnt();
    Object* ownerObject = nullptr;

    _vec3 tntOffset = { 0.f, 5.f, 0.f };

    bool throwTnt = false;
    const float throwSpeed = 10.f;
    _vec3 throwDirection = { 0.f,0.f,0.f };

    float TntTime = 0.f;
    const float TntBoom = 3.f;
    const float TntRange = 10.f;
};

