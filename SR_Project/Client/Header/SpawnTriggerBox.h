#pragma once
#include "Object.h"

class Spawner;
class SpawnTriggerBox :
    public Object
{
protected:
    SpawnTriggerBox(ObjectManager* owner, ObjectType objType);
    virtual ~SpawnTriggerBox();

public:
    static SpawnTriggerBox* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

public:
    void SetCollisionSize(_vec3 size);
    void SetTriggerPosition(_vec3 pos);
    void AddSpawner(SpawnType type, _vec3 pos, _vec3 rot);

    void RegisterCallBack(std::function<void()> callback) { callBack = callback; }
    void OnCollisionEnter(Object* other);

private:
    void Free()override;

    vector<Spawner*> spawners;
    _bool           SpawnOn = false;

    std::function<void()> callBack = nullptr;
};

