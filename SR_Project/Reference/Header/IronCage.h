#pragma once
#include "DynamicBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL IronCage : public DynamicBlock
{
    friend class DynamicBlock;
private:
    IronCage(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, DynamicBlockRot rot, int Count);
    virtual ~IronCage();

public:
    static IronCage* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col, DynamicBlockRot rot, int Count);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType, DynamicBlockRot rot);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::wstring& mtrl, string str);
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);

public:
    void Operate();

private:
    void Free() override;

private:
    bool First = true;
    float totTrans = 0.f;
    float Speed = 5.f;
};
END