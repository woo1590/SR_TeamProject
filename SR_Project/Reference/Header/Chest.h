#pragma once
#include "DynamicBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL Chest : public DynamicBlock
{
    friend class DynamicBlock;
private:
    Chest(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockRot rot);
    virtual ~Chest();

public:
    static Chest* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type, DynamicBlockRot rot);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType, DynamicBlockRot rot);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::wstring& mtrl, string str);
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);

public:
    void Generate(_float dt);
    float EaseOutBack(float t);
    void Operate();

private:
    void Free() override;

private:
    bool First = true;
    float firstTime = 0.f;

    float Angle = 0.f;
    float totAngle = 0.f;
    float rotSpeed = 5.f;

    std::unordered_map<std::string, _vec3> PartScales;
};
END