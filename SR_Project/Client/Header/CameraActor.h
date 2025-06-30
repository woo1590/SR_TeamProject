#pragma once
#include "Object.h"

class CameraActor :
    public Object
{
protected:
    CameraActor(ObjectManager* owner, ObjectType objType = ObjectType::Camera);
    virtual ~CameraActor();

public:
    HRESULT Ready_Object()override;

protected:
    void Free()override;
};

