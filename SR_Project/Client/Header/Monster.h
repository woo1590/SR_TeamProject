#pragma once
#include "BaseCharacter.h"

enum MonsterState { Idle, Walk, Attack, Die, };

struct Animation
{
    _float Start;       //start angle;
    _float End;         //end angle;

    _float TotalTime;   //total time
    _float ElapsedTime; //elapsed time

    _float DelayTime;   //delay time;

    _bool IsRunning;
    _bool IsEnd;        //animation end
};

class Monster :
    public BaseCharacter
{
protected:
    Monster(ObjectManager* owner, ObjectType objType);
    virtual ~Monster();

public:
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void MoveTo(_vec3* dir, _float dt) override;
    _vec3 RotateTo(_vec3* dir, float dt);
    void Attack(Object* target) override;
    void Die() override;
    
    _bool IsAttackStart() const { return AttackAnim.IsRunning; }
    _bool IsAttackFinish() const { return AttackAnim.IsEnd; }

    _bool IsDeadStart() const { return DieAnim.IsRunning; }
    _bool IsDeadFinish() const { return DieAnim.IsEnd; }

    _float GetHp() const { return Hp; }           //modify

protected:
    virtual void InitAnimation();
    virtual void PlayAnimation(_float dt);

    virtual void PlayIdle(_float dt);
    virtual void PlayWalk(_float dt);
    virtual void PlayAttack(_float dt);
    virtual void PlayDie(_float dt);

protected:
    void Free() override;

protected:
    MonsterState        State = MonsterState::Idle;
    _float              Speed = 7.f;

    Animation           WalkAnim;
    Animation           AttackAnim;
    Animation           DieAnim;

    _float              Hp = 100.f;
};

