#pragma once
#include "StaticBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL BasicBlock : public StaticBlock
{
    friend class StaticBlock;
private:
    BasicBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    virtual ~BasicBlock();

public:
    static BasicBlock* Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free() override;
};
END