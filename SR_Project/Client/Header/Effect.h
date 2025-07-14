#pragma once
#include "Object.h"

namespace Engine
{
    class TransformComponent;
}

class Effect :
    public Object
{
protected:
    Effect(ObjectManager* owner, ObjectType objType);
    virtual ~Effect();

public:
    void SetDeadTime(_float time) { deadTime = time; }
    void SetOwner(TransformComponent* o) { effectOwner = o; }
protected:
    TransformComponent* effectOwner = nullptr;
    _float deadTime = 0.f;
    _float timer = 0.f;
};

