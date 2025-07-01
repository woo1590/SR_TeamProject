#pragma once
#include "BaseCharacter.h"

enum MonsterState { Idle, Walk, Attack, Die, };
class Monster :
    public BaseCharacter
{
protected:
    Monster(ObjectManager* owner, ObjectType objType);
    virtual ~Monster();

public:
    static Monster* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void MoveTo(_vec3* dir) override;
    void Attack(Object* target) override;
    
    _bool IsAttack() { return IsAttacking; }
    _bool IsAttackFinish() { return IsAttackFinished; }

protected:
    virtual void PlayAnimation(_float dt);

    virtual void PlayIdle(_float dt);
    virtual void PlayWalk(_float dt);
    virtual void PlayAttack(_float dt);
    virtual void PlayDie(_float dt);

protected:
    void Free() override;

protected:
    MonsterState        State = MonsterState::Idle;
    _float              Speed = 0.2f;
    
    _float              WalkTime = 0.f;
    _float              AttackTime = 0.f;

    _float              AttackDelay = 2.f;

    _bool               IsAttacking = false;
    _bool               IsAttackFinished = true;
};

