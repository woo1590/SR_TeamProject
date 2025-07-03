#pragma once
#include "Monster.h"
class Zombie :
    public Monster
{
protected:
    Zombie(ObjectManager* owner, ObjectType objType);
    virtual ~Zombie();

public:
    static Zombie* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void MoveTo(_vec3* dir, _float dt) override;
    void RotateTo(_vec3* dir, float dt) override;
    void Attack(Object* target) override;
    void Die() override;

    _bool IsAttackStart() const { return AttackAnim.IsRunning; }
    _bool IsAttackFinish() const { return AttackAnim.IsEnd; }

    _bool IsDeadStart() const { return DieAnim.IsRunning; }
    _bool IsDeadFinish() const { return DieAnim.IsEnd; }

    _float GetHp() const { return Hp; }           //modify

protected:
    void InitAnimation() override;
    void PlayAnimation(_float dt) override;

    void PlayIdle(_float dt) override;
    void PlayWalk(_float dt) override;
    void PlayAttack(_float dt) override;
    void PlayDie(_float dt)override;

    void OnCollisionStay(Object* other) override;

protected:
    void Free() override;
};

