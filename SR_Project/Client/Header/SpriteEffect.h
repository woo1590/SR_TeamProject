#pragma once
#include "Effect.h"

class SpriteEffect :
    public Effect
{
private:
    SpriteEffect(ObjectManager* owner, ObjectType objType);
    virtual ~SpriteEffect();

public:
    static SpriteEffect* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object();

};

