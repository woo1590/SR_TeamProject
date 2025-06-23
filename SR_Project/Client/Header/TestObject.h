#pragma once
#include "Object.h"

class TestObject :
    public Object
{
private:
    TestObject(ObjectManager* owner, ObjectType objType);
    virtual ~TestObject();

public:
    static TestObject* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;

};

