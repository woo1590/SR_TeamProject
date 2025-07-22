#pragma once
#include "DynamicBlock.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL MapTable : public DynamicBlock
{
    friend class DynamicBlock;
private:
    MapTable(ObjectManager* owner, ObjectType objType, DynamicBlockType type);
    virtual ~MapTable();

public:
    static MapTable* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType type);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void SetMaterial(const std::string& mtrl, string str);
    void SetScale(float scale);
    void SetPosition(_vec3 position, string str);
    void SetRotation(_vec3 rotation, string str);

private:
    void Free() override;

private:
    std::unordered_map<std::string, _vec3> PartScales;
};
END