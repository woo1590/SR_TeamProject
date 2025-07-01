#pragma once
#include "CameraActor.h"

class ThirdCam :
    public CameraActor
{
private:
    ThirdCam(ObjectManager* owner);
    virtual ~ThirdCam();

public:
    static ThirdCam* Create(ObjectManager* owner);
    HRESULT Ready_Object()override;

    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void SetTarget(Object* target);
    void SetOffset(_vec3 offset);
private:
    void Free()override;
};

