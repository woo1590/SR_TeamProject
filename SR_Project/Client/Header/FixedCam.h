#pragma once
#include "CameraActor.h"

class FixedCam :
    public CameraActor
{
private:
    FixedCam(ObjectManager* owner);
    virtual ~FixedCam();

public:
    FixedCam* Create(ObjectManager* owner);
    HRESULT Ready_Object()override;

    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};

