#pragma once
#include "Item.h"
class Sword :
    public Item
{
    Sword(ObjectManager* owner, ObjectType objType);
    virtual ~Sword();
    void Free() override;
public:
    static Sword* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetCollisionEnter(Object* other);

    void PlayerSwordInfo();

    void FindNextTarget(Object* targetMonster);

    std::vector<Object*> targetMonsters;
    _float attackTime = 0.f;
    _float delayTimer = 0.f;
    const _float damageTerm = 0.1f;

    _int preTarget = 0;
    _vec3 prePos = { 0.f,0.f,0.f };
    _vec3 curPos = { 0.f,0.f,0.f };

    std::vector<Object*> hitMonsters;
};

