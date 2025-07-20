#pragma once
#include "BaseCharacter.h"
#include "DialogManager.h"

class NPCShop :
    public BaseCharacter
{
protected:
    NPCShop(ObjectManager* owner, ObjectType objType);
    virtual ~NPCShop();

public:
    static NPCShop* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void Talk(DialogManager* dialogMgr);

private:
    void InitTransform();
    void InitCollision();
    void InitPhysics();
    void InitDialog();

protected:
    void Free() override;

private:
    struct DialogSet
    {
        function<bool()> condition;
        vector<DialogLine> lines;
        function<void()> onFinish;
    };
    vector<DialogSet> dialogSets;
};

