#pragma once
#include "DynamicBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL Bridge : public DynamicBlock
{
    friend class DynamicBlock;
private:
    Bridge(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockCol DynamicBlockCol, int Count);
    virtual ~Bridge();

public:
    static Bridge* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockCol DynamicBlockCol, int Count);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::string& mtrl, string str);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);

public:
    void Operate();

private:
    void Free() override;

private:
    bool First = true;

    float Angle = 0.f;
    float totAngle = 0.f;
    float rotSpeed = 5.f;
};
END