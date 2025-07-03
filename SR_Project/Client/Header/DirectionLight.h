#pragma once
#include "Object.h"

class DirectionLight :
    public Object
{
private:
    DirectionLight(ObjectManager* owner, ObjectType objType);
    virtual ~DirectionLight();

public:
    static DirectionLight* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    
private:
    void Free()override;
};

