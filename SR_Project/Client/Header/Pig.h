#pragma once
#include "Object.h"

class Bone;
class Pig :
    public Object
{
private:
    Pig(ObjectManager* owner, ObjectType objType);
    virtual ~Pig();

public:
    static Pig* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void InitTransform();
    void InitCollision();
    void InitPhysics();
    void MoveTo(_float dt);
    void PlayAnimation(_float dt);

private:
    void Free() override;

private:
    unordered_map<string, Bone*>    Bones;
    _float                          Scale =0.3f;
    
    _float                          ElapsedTime = 0.f;
};

