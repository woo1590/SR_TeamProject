#pragma once
#include "Object.h"
#include "TransformComponent.h"

class ENGINE_DLL TestBlock  : public Object
{
    friend class Object;
private:
    TestBlock(ObjectManager* owner, ObjectType objType, BlockType blockType, BlockDir blockDir);
    virtual ~TestBlock();

public:
    static TestBlock* Create(ObjectManager* owner, ObjectType objType, BlockType blockType, BlockDir blockDir);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
    
public:
    _vec3 GetPosition() { return AddComponent<TransformComponent>()->GetPosition(); }

private:
    void Free() override;

private:
    BlockDir m_eDir;
    BlockType m_eType;
};