#pragma once
#include "Object.h"
#include "Engine_Struct.h"

BEGIN(Engine)

class StaticBlock;
class ChunkMesh;
class ENGINE_DLL Chunk : public Object
{
private:
	Chunk(ObjectManager* owner, ObjectType objType, int ChunkX, int ChunkZ);
	virtual ~Chunk();

public:
	static Chunk * Create(ObjectManager* owner, ObjectType objType, int ChunkX, int ChunkZ);
	HRESULT Ready_Object();
	void Update(_float dt);

	void AddBlock(int lx, int ly, int lz, SB sb);
	void BuildMesh();
	int Index(int lx, int ly, int lz);
private:
	void Free()override;

	_bool Is_Dirty = false;
	int ChunkX;
	int ChunkY = 0;
	int ChunkZ;
	std::vector<SB> Blocks;
	ChunkMesh* mesh = nullptr;
};
END