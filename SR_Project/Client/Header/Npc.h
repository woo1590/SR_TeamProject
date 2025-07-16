#pragma once
#include "BaseCharacter.h"
#include "DialogManager.h"


class Npc :
    public BaseCharacter
{
protected:
    Npc(ObjectManager* owner, ObjectType objType);
    virtual ~Npc();

public:
    static Npc* Create(ObjectManager* owner, ObjectType objType);
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
        vector<wstring> lines;
        function<void()> onFinish;
    };
    vector<DialogSet> dialogSets;
};

