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
    virtual void MoveTo(_vec3* dir, _float dt);
    virtual void HitMoveTo(_vec3* dir, _float dt);
    virtual void Attack(Object* target);
    virtual void Die() {};
    virtual void Add_Bone(string str, ObjectType objType, Object* parent, const wstring& mtrl);
    void PlayKnockBack(_vec3 dir, _float attack, _float dt);

protected:
    void SetMaterial(const std::wstring& mtrl, string str = "Body", RENDER_ID id = RENDER_ID::Render_NonAlpha);
    void SetScale(_vec3 scale, string str = "Body");
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str = "Body");
    void SetRotation(_vec3 rotation, string str = "Body");
    void SetTranslate(_vec3 translate, string str = "Body");
    void SetWeapon(Object* parent,ObjectType objType, const wstring& mtrl);

protected:
    void Free() override;

protected:
    std::unordered_map<string, Object*> Bones;
    float                               Scale = 0.75f; // 1 -> 0.75

    Object*                             Weapon = nullptr;
};