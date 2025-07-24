#pragma once
#include "Monster.h"
class JungleSkeleton :
    public Monster
{
protected:
    JungleSkeleton(ObjectManager* owner, ObjectType objType);
    virtual ~JungleSkeleton();

public:
    static JungleSkeleton* Create(ObjectManager* owner, ObjectType objType);
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
    void PlayHit(float dt) override;
    void PlayDie(_float dt)override;

    void OnCollisionStay(Object* other) override;

protected:
    void Free() override;

private:
    _float* NearDistance = nullptr;

    _float deadTimer = 0.f;
    _float isDetachBones = false;
};

