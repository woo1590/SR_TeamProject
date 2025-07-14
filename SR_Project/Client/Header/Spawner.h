#pragma once

#include "Object.h"
#include "Scene.h"

class Monster;

class Spawner : public Object
{
private:
	Spawner(Scene* scene, ObjectManager* objMgr, ObjectType objType)
		:Object(objMgr, objType), scene(scene) {}

public:
	static Spawner* Create(Scene* scene, ObjectManager* objMgr, SpawnType type, _vec3 pos = {}, _vec3 rot = {});
	Monster* Spawn();

private:
	Scene* scene;
	SpawnType spawnType{};
	_vec3 pos{}, rot{};
};
