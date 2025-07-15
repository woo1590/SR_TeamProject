#pragma once
#include "Monster.h"
class Ender :
    public Monster
{
protected:
    Ender(ObjectManager* owner, ObjectType objType);
    virtual ~Ender();

public:
    static Ender* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    void InitTransform(ObjectType objType);
    void InitTree();
    void InitAnimation() override;
    void PlayAnimation(_float dt) override;

protected:
    void Free() override;
};

