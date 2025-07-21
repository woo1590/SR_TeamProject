#pragma once
#include "Item.h"
class Armor :
    public Item
{
    Armor(ObjectManager* owner, ObjectType objType);
    virtual ~Armor();
    void Free() override;
public:
    static Armor* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    std::unordered_map<string, Object*> GetBones();
    void SetOwner(ObjectManager* owner)override;
private:
    std::unordered_map<string, Object*> Bones;
    _float Scale = 1.f;
};
