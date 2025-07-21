#pragma once
#include "Object.h"

BEGIN(Engine)

class ENGINE_DLL CollisionBlock :
    public Object
{
private:
    CollisionBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type);
    virtual ~CollisionBlock();

public:
    static CollisionBlock* Create(ObjectManager* owner, ObjectType objType, StaticBlockType type);
    HRESULT Ready_Object()override;

    void SetOwner(ObjectManager* owner)override { this->owner = owner; }
    StaticBlockType GetType()const { return blockType; }
private:
    void Free()override;

    StaticBlockType blockType = Air;
};

END