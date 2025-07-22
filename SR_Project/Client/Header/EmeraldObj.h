#pragma once
#include "Object.h"
class EmeraldObj :
    public Object
{
private:
    EmeraldObj(ObjectManager* owner, ObjectType objType);
    virtual ~EmeraldObj();

public:
    static EmeraldObj* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void Throw();

private:
    void Free() override;

};

