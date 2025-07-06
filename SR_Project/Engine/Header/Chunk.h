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
    void AddBlock(ObjectManager* objectMgr, int Count);

    void SetSB(SB sb) { CD.sBlocks.push_back(sb); }

    const CHUNK& GetChunkData() const { return CD; }

private:
    void Free()override;

private:
    CHUNK CD;
};
END