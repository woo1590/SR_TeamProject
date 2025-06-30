#pragma once
#include "CameraActor.h"

class FirstCam :
    public CameraActor
{
private:
    FirstCam(ObjectManager* owner);
    virtual ~FirstCam();

public:
    static FirstCam* Create(ObjectManager* owner);
    HRESULT Ready_Object()override;

    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

