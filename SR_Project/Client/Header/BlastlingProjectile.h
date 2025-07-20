#pragma once
#include "Object.h"
class BlastlingProjectile :
    public Object
{
protected:
    BlastlingProjectile(ObjectManager* owner, ObjectType objType);
    virtual ~BlastlingProjectile();

public:
    static BlastlingProjectile* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void CheckGround();
    void OnCollisionEnter(Object* other);
    void UpdateFire(_float dt);

protected:
    void Free() override;

public:
    void    SetVisible(_bool visible);
    void    FireProjectile(_vec3 dir);

private:
    _float ElapsedTime = 0.f;
    _bool Visible = false;
    _bool Fire = false;
    _vec3 FireDir;
};

