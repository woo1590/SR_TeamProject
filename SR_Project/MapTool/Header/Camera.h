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

public:
    void SetPosition(_vec3 pos);
    void LookAt(_vec3 look);

private:
    void Free()override;
};