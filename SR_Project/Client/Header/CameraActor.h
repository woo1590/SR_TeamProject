#pragma once
#include "Object.h"

class CameraActor :
    public Object
{
private:
    CameraActor(ObjectManager* owner, ObjectType objType);
    virtual ~CameraActor();

public:
    static CameraActor* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

