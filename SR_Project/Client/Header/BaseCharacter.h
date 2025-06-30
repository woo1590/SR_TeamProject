#pragma once
#include "Object.h"
class BaseCharacter :
    public Object
{
protected:
    BaseCharacter(ObjectManager* owner, ObjectType objType);
    virtual ~BaseCharacter();

public:
    static BaseCharacter* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    virtual void MoveTo(_vec3* dir);
    virtual void Attack(Object* target);

protected:
    void SetMaterial(const std::wstring& mtrl, string str = "Body");
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str = "Body");
    void SetRotation(_vec3 rotation, string str = "Body");

protected:
    void Free() override;

protected:
    std::unordered_map<string, Object*> Bones;
    float                               Scale = 1.f;
};