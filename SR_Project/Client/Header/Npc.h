#pragma once
#include "BaseCharacter.h"
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

private:
    void InitTransform();
    void InitCollision();
    void InitPhysics();

protected:
    void Free() override;

};

