#pragma once
#include "Effect.h"
class StatikkEffect :
    public Effect
{
    StatikkEffect(ObjectManager* owner, ObjectType objType, _vec3 startPos, _vec3 endPos);
    virtual ~StatikkEffect();

public:
    static StatikkEffect* Create(ObjectManager* owner, ObjectType objType, _vec3 startPos, _vec3 endPos);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;

private:
    void Free()override;

    _vec3 startPos = { 0.f,0.f,0.f };
    _vec3 endPos = { 0.f,0.f,0.f };

    _float Scale = 0.f;
};

