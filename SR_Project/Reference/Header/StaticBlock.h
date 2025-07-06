#pragma once
#include "Object.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL StaticBlock : public Object
{
    friend class Object;
protected:
    StaticBlock(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    virtual ~StaticBlock();

public:
    static Object* Create(ObjectManager* owner, ObjectType objType, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    
public:
    _vec3 GetPosition() { return AddComponent<TransformComponent>()->GetPosition(); }

private:
    void Free() override;

protected:
    StaticBlockType Type;
    StaticBlockAxis Axis;
    StaticBlockRot Rot;
    StaticBlockUsage Usage;
};
END