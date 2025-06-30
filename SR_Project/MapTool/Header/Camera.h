#pragma once
#include "Object.h"

class Camera : public Object
{
private:
	Camera(ObjectManager* owner, ObjectType objType);
	virtual ~Camera();

public:
    static Camera* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};