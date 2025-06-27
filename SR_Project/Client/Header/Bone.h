#pragma once
#include "Object.h"
class Bone :
    public Object
{
private:
    Bone(ObjectManager* owner, ObjectType objType);
    virtual ~Bone();

public:
    static Bone* Create(ObjectManager* owner, ObjectType objType, _vec3 scale, Object* parent, const std::wstring& mtrl);
    HRESULT Ready_Object(Object* Parent, _vec3 scale, const std::wstring& mtrl);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;
};