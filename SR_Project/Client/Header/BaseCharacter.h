#pragma once
#include "Object.h"
class BaseCharacter :
    public Object
{
private:
    BaseCharacter(ObjectManager* owner, ObjectType objType);
    virtual ~BaseCharacter();

public:
    static BaseCharacter* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;

private:
    void SetMaterial(string str, const std::wstring& mtrl);
    void SetScale(string str, _vec3 scale);
    void SetPosition(string str, _vec3 position);
    void SetRotation(string str, _vec3 rotation);

private:
    std::unordered_map<string, Object*> Bones;
};