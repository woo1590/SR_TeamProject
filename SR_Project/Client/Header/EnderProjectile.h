#pragma once
#include "Object.h"
class EnderProjectile :
    public Object
{
protected:
    EnderProjectile(ObjectManager* owner, ObjectType objType);
    virtual ~EnderProjectile();

public:
    static EnderProjectile* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void PlayScaleAnimation(_float dt);
    void CheckGround();

protected:
    void Free() override;

public:
    void    SetVisible(_bool visible);
    void    FireProjectile(_vec3 dir);

private:
    _float  ElapsedTime = 0.f;
    _bool   Visible = false;
};

