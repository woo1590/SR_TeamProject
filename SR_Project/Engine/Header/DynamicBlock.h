#pragma once
#include "Object.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL DynamicBlock : public Object
{
    friend class Object;
private:
    DynamicBlock(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir);
    virtual ~DynamicBlock();

public:
    static DynamicBlock* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free() override;

protected:
    DynamicBlockDir Dir;
    DynamicBlockType Type;
    std::unordered_map<string, Object*> Parts;
};
END