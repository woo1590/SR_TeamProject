#pragma once
#include "CameraActor.h"

class FixedCam :
    public CameraActor
{
private:
    FixedCam(ObjectManager* owner);
    virtual ~FixedCam();

public:
    static FixedCam* Create(ObjectManager* owner);
    HRESULT Ready_Object()override;

    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void SetPosition(_float x, _float y, _float z);
    void SetForward(_float x, _float y, _float z);
private:
    void Free()override;
};

