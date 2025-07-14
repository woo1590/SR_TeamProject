#pragma once
#include "Boss.h"
class RedGolem :
    public Boss
{
    friend class ObjectPool<RedGolem>;

protected:
    RedGolem(ObjectManager* owner, ObjectType objType);
    virtual ~RedGolem();

public:
    static RedGolem* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void MoveTo(_vec3* dir, _float dt) override;
    void RotateTo(_vec3* dir, float dt) override;
    void Attack(Object* target) override;
    void Die() override;
    void Hit(_vec3 dir, _float power) override;

protected:
    void InitTransform(ObjectType objType);
    void InitTree();
    void InitAnimation() override;
    void PlayAnimation(_float dt) override;

    void PlayIdle(_float dt) override;
    void PlayWalk(_float dt) override;
    void PlayAttack(_float dt) override;
    void PlayDie(_float dt) override;
    void PlayHit(_float dt) override;

    void OnCollisionStay(Object* other) override;

private:
    void PlayLeftAttack(_float dt);
    void PlayRightAttack(_float dt);
    void PlaySuperAttack(_float dt);
    void PlaySuperAttackDelay(_float dt);

    //Object Pool
    void InitProjectile(ObjectType objType);

protected:
    void Free() override;

private:
    Animation           LeftAttackAnim;
    Animation           RightAttackAnim;
    Animation           SuperAttackAnim;

    vector<Object*>     BoxProjectile;

    _float              SpawnTime = 0.f;
    _int                Index = 0;

    _float*             AttackTimer = nullptr;
    _int*               AttackNum = nullptr;

};

