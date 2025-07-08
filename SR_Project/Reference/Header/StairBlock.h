#pragma once
#include "StaticBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL StairBlock : public StaticBlock
{
    friend class StaticBlock;
private:
    StairBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    virtual ~StairBlock();

public:
    static StairBlock* Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::string& mtrl, string str);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);

private:
    void Free() override;

private:
    std::unordered_map<string, Object*> Parts;
};
END