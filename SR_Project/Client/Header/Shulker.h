#pragma once
#include "Monster.h"
class Shulker :
    public Monster
{
protected:
    Shulker(ObjectManager* owner, ObjectType objType);
    virtual ~Shulker();

public:
    static Shulker* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Attack(Object* target) override;
    void Die() override;

protected:
    void InitTransform(ObjectType objType);
    void InitTree();
    void InitAnimation() override;
    void PlayAnimation(_float dt) override;

    void PlayIdle(_float dt) override;
    void PlayDie(_float dt) override;

protected:
    void Free() override;
};

