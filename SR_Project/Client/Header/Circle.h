#pragma once
#include "Object.h"
class Circle :
    public Object
{
protected:
    Circle(ObjectManager* owner, ObjectType objType);
    virtual ~Circle();

public:
    static Circle* Create(ObjectManager* owner, ObjectType objType, Object* parent);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType, Object* parent);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void SetOn(_bool On);

protected:
    void Free() override;
};

