#pragma once
#include "Effect.h"
class FireworkEffect :
    public Effect
{
    FireworkEffect(ObjectManager* owner, ObjectType objType, Object* _onPos);
    virtual ~FireworkEffect();

public:
    static FireworkEffect* Create(ObjectManager* owner, ObjectType objType, Object* _onPos);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _float Scale = 0.f;
    _vec3 startPos = { 0.f,0.f,0.f };
    _vec3 moveDir = { 0.f,0.f,0.f };
    _float moveSpeed = 24.f;
};

