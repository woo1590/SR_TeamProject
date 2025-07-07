#pragma once
#include "Object.h"
class GolemProjectile :
    public Object
{
protected:
    GolemProjectile(ObjectManager* owner, ObjectType objType);
    virtual ~GolemProjectile();

public:
    static GolemProjectile* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void SetOn(_bool On);

private:
    void PlayScaleAnimation(_float dt);

protected:
    void Free() override;

private:
    _float          ElapsedTime = 0.f;
    _bool           IsOn = false;
};

