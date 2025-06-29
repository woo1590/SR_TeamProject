#pragma once
#include "Object.h"
class Player : public Object
{
private:
    Player(ObjectManager* owner, ObjectType objType);
    virtual ~Player();

public:
    static Player* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;

private:
    void SetMaterial(string str, const std::wstring& mtrl);
    void SetScale(string str, _vec3 scale);
    void SetPosition(string str, _vec3 position);
    void MovePosition(_vec3 moveVec);
    void SetRotation(string str, _vec3 rotation);

private:
    std::unordered_map<string, Object*> Bones;
};