#pragma once
#include "Base.h"
#include "Engine_Struct.h"
#include "ObjectManager.h"

BEGIN(Engine)
class ENGINE_DLL Chunk : public Base
{
private:
    Chunk(int chunkX, int chunkZ);
    virtual ~Chunk();

public:
    static Chunk* Create(int chunkX, int chunkZ);
    void AddBlock(const _vec3& pos, StaticBlockType type, StaticBlockAxis axis, StaticBlockRot rot, StaticBlockUsage usage);
    void Render();

    const CHUNK& GetChunkData() const { return CD; }

private:
    void Free()override;

private:
    CHUNK CD;
};
END