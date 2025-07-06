#pragma once
#include "BaseCharacter.h"

enum MonsterState { Idle, Walk, AttackReady, Attack, Hit, Die, };

struct Animation
{
    _float Start;       //start angle;
    _float End;         //end angle;

    _float TotalTime;   //total time
    _float ElapsedTime; //elapsed time

    _float DelayTime;   //delay time;

    _bool IsRunning = false;
    _bool IsEnd = false;        //animation end
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
    virtual void RotateTo(_vec3* dir, float dt);
    void Attack(Object* target) override;
    void Die() override;
    virtual void Hit(_vec3 dir, _float power);
    
    _bool IsAttackStart() const { return AttackAnim.IsRunning; }
    _bool IsAttackFinish() const { return AttackAnim.IsEnd; }

    _bool IsDeadStart() const { return DieAnim.IsRunning; }
    _bool IsDeadFinish() const { return DieAnim.IsEnd; }

    _bool IsHitStart() const { return HitAnim.IsRunning; }
    _bool IsHitFinish() const { return HitAnim.IsEnd; }

    virtual _float GetHp();

protected:
    virtual void InitAnimation();
    virtual void PlayAnimation(_float dt);

    virtual void PlayIdle(_float dt);
    virtual void PlayWalk(_float dt);
    virtual void PlayAttack(_float dt);
    virtual void PlayDie(_float dt);
    virtual void PlayHit(_float dt);

    virtual void OnCollisionStay(Object* other);

protected:
    void Free() override;

protected:
    MonsterState        State = MonsterState::Idle;
    _float              Speed = 5.f;
    _float*             Distance = nullptr;
    _bool*              IsHit = nullptr;

    Animation           WalkAnim;
    Animation           AttackAnim;
    Animation           DieAnim;
    Animation           HitAnim;

    _vec3               HitDir;
    _float              HitPower;
};

