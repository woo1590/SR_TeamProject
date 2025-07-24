#pragma once
#include "Object.h"
class CheckPoint :
    public Object
{
    CheckPoint(ObjectManager* owner, ObjectType objType, LOADID _scene, _int _num);
    virtual ~CheckPoint();

public:
    static CheckPoint* Create(ObjectManager* owner, ObjectType objType, LOADID _scene, _int _num);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void SetCollisionSize(_vec3 size);
    void SetTriggerPosition(_vec3 pos);

    void OnCollisionEnter(Object* other);

private:
    void Free()override;

    LOADID scene = LOADID::Count;
    _int num = -1;
};

