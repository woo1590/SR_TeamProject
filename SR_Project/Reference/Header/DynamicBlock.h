#pragma once
#include "Object.h"
#include "TransformComponent.h"

BEGIN(Engine)
class ENGINE_DLL DynamicBlock : public Object
{
    friend class Object;
protected:
    DynamicBlock(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir, int Count);
    virtual ~DynamicBlock();

public:
    static Object* Create(ObjectManager* owner, ObjectType objType, DynamicBlockType dynamicBlockType, DynamicBlockDir dynamicBlockDir, int Count);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void LoadLink();
    void AddID(int addID) { LinkedID.push_back(addID); }
    void SetLinkedID(vector<int> IDs) { LinkedID = IDs; }

    int GetID() { return ID; }
    int GetType() { return Type; }
    int GetCount() { return Count; }
    bool GetTrigger() { return Trigger; }
    vector<int> GetIDVec() { return LinkedID; }

    void SetID(int id) { ID = id; }
    void SetCount(int count) { Count = count; }

private:
    void Free() override;

protected:
    bool Activate = false;
    bool Trigger = false;
    int Count = 0;
    float Scale = 1.f;

    DynamicBlockDir Dir;
    DynamicBlockType Type;
    std::unordered_map<string, Object*> Parts;

    int ID = -1;
    vector<int> LinkedID;
    vector<Object*> LinkedObject;
};
END