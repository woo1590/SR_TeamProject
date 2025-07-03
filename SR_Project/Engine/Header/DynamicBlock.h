#pragma once
#include "Object.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL DynamicBlock : public Object
{
    friend class Object;
protected:
    DynamicBlock(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir, int Count);
    virtual ~DynamicBlock();

public:
    static Object* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir, int Count);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free() override;

protected:
    bool On = false;
    int Count = 0;
    float Scale = 1.f;

    DynamicBlockDir Dir;
    DynamicBlockType Type;
    std::unordered_map<string, Object*> Parts;
};
END