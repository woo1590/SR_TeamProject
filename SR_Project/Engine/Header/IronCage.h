#pragma once
#include "DynamicBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL IronCage : public DynamicBlock
{
    friend class DynamicBlock;
private:
    IronCage(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockDir DynamicBlockDir, int Count);
    virtual ~IronCage();

public:
    static IronCage* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockDir DynamicBlockDir, int Count);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::wstring& mtrl, string str);
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);

public:
    void SetOn() { On = true; }
    void Operate();

private:
    void Free() override;

private:
    float totTrans = 0.f;
    float Speed = 5.f;
};
END