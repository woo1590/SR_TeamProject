#pragma once
#include "Effect.h"
class LaserEffect :
    public Object
{
private:
    LaserEffect(ObjectManager* owner, ObjectType objType);
    virtual ~LaserEffect();

public:
    static LaserEffect* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object();
    void Update(_float dt);
    void Late_Update(_float dt)override;

private:
    void Free()override;

public:
    void    SetActive(_bool active);
    void    OnCollisionStay(Object* other);

private:
    _float  ElapsedTime = 0.f;
    _bool   Active;
    _vec3   ColSize;
};

