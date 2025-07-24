#pragma once
#include "EnginePCH.h"

enum class LOADID
{
	Village,
	Stage1,
	Stage2,
	Count
};

enum class SpawnType { Zombie, Skeleton, JungleSkeleton, Creeper, JungleZombie, Slime, PurpleSlime, Ender, RedGolem, Blastling, Shulker };

struct WayPoint
{
	_vec3 position;
	_vec3 lookDir;

	WayPoint() = default;
	WayPoint(_vec3 pos, _vec3 dir)
	{
		position = pos;
		D3DXVec3Normalize(&lookDir, &dir);
	}
};