#pragma once
#include "DynamicBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL Lever : public DynamicBlock
{
    friend class DynamicBlock;
private:
    Lever(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col);
    virtual ~Lever();

public:
    static Lever* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockCol col);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::wstring& mtrl, string str);
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);
    static void ResetID() { totID = 0; }

public:
    void Operate();

private:
    void Free() override;

private:
    static int totID;

    float Angle = 0.f;
    float totAngle = 0.f;
    float rotSpeed = 5.f;
};
END