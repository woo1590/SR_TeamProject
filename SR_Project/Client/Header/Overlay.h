#pragma once
#include "Object.h"

class Overlay :
    public Object
{
private:
    Overlay(ObjectManager* owner, ObjectType objType);
    virtual ~Overlay();

public:
    static Overlay* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void SetFadeIn(_bool fade) { fadeIn = fade; }
    void SetDuration(_float duration) { this->duration = duration; }
    _bool IsFinished()const { return isFinish; }
private:
    void Free()override;

    _float alpha = 1.f;

    _float duration = 0.f;
    _float timer = 0.f;
    _bool isFinish = false;
    _bool fadeIn = true;
};

