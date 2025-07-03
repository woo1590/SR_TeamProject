#pragma once
#include "Object.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL StaticBlock : public Object
{
    friend class Object;
private:
    StaticBlock(ObjectManager* owner, ObjectType objType, StaticBlockType staticBlockType, StaticBlockDir staticBlockDir);
    virtual ~StaticBlock();

public:
    static StaticBlock* Create(ObjectManager* owner, ObjectType objType, StaticBlockType staticBlockType, StaticBlockDir staticBlockDir);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    
public:
    _vec3 GetPosition() { return AddComponent<TransformComponent>()->GetPosition(); }

private:
    void Free() override;

private:
    StaticBlockDir Dir;
    StaticBlockType Type;
};
END