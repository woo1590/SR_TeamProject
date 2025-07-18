#pragma once
#include "Object.h"

class FireBlock :
    public Object
{
private:
    FireBlock(ObjectManager* owner, ObjectType objType);
    virtual ~FireBlock();

public:
    static FireBlock* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    void SetColor(_vec3 color);
    void SetDeadTime(_float time) { deadTime = time; }
private:
    void Free()override;

    _float timer = 0.f;
    _float speed = 16.f;
    _vec3 color{ 0.2f,0.f,0.5f };
    _uint totalFrame = 16;
    _uint currFrame = 0;
    _float deadTime = 0.f;
    _float deadTimer = 0.f;
};

