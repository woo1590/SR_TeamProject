#pragma once
#include "Object.h"

BEGIN(Engine)

class ENGINE_DLL CollisionBlock :
    public Object
{
private:
    CollisionBlock(ObjectManager* owner, ObjectType objType);
    virtual ~CollisionBlock();

public:
    static CollisionBlock* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object()override;

    void SetOwner(ObjectManager* owner)override { this->owner = owner; }
private:
    void Free()override;
};

END