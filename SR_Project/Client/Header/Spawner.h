#pragma once
#include "Object.h"

enum class SpawnType {Zombie, Skeleton, Creeper, RedGolem,};

class Monster;

class Spawner :
    public Object
{
protected:
    Spawner(ObjectManager* owner, ObjectType objType);
    virtual ~Spawner();

public:
    static Spawner* Create(ObjectManager* owner, ObjectType objType,
        SpawnType type, _vec3 position, _vec3 rotation);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType,
        SpawnType type, _vec3 position, _vec3 rotation);

public:
    Monster* Spawn();

private:
    _vec3 Position;
    _vec3 Rotation;
    SpawnType spawntype;
};

