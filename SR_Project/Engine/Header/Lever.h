#pragma once
#include "Object.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL Lever : public Object
{
    friend class Object;
private:
    Lever(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockDir DynamicBlockDir);
    virtual ~Lever();

public:
    static Lever* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType DynamicBlockType, DynamicBlockDir DynamicBlockDir);
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
    bool On = false;
    float Scale = 1.f;
    float Angle = 0.f;
    float totAngle = 0.f;
    float rotSpeed = 5.f;
    DynamicBlockDir Dir;
    DynamicBlockType Type;
    std::unordered_map<string, Object*> Parts;
};
END