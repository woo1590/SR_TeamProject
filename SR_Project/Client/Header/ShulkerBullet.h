#pragma once
#include "Object.h"

enum BulletDir{L,R,U,D, };

class Bone;
class ShulkerBullet :
    public Object
{
protected:
    ShulkerBullet(ObjectManager* owner, ObjectType objType);
    virtual ~ShulkerBullet();

public:
    static ShulkerBullet* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void    SetActive(bool active);
    void    CheckDir();
    void    MoveToDir(_float dt);

private:
    void OnCollisionEnter(Object* other);

protected:
    void Free() override;

private:
    vector<Bone*> Bones;
    _bool       Active;
    _int        RotateCount = 0;
    BulletDir   CurDir;
    BulletDir   PreDir;

    _float      ActiveTimer = 0.f;
};

