#pragma once
#include "Object.h"
#include "TransformComponent.h"

class TestBlock : public Object
{
    friend class Object;
private:
    TestBlock(ObjectManager* owner, ObjectType objType, BlockType blockType);
    virtual ~TestBlock();

public:
    static TestBlock* Create(ObjectManager* owner, ObjectType objType, BlockType blockType);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    
public:
    _vec3 GetPosition() { return AddComponent<TransformComponent>()->GetPosition(); }

private:
    void Free() override;

private:
    BlockType m_eType;
};