#pragma once
#include "Base.h"
#include "Engine_Struct.h"
#include "ObjectManager.h"

BEGIN(Engine)
class ENGINE_DLL Chunk : public Base
{
private:
    Chunk(CHUNK chunkData);
    virtual ~Chunk();

private:
    HRESULT Ready_Object(CHUNK chunkData);

public:
    static Chunk* Create(CHUNK chunkData);
    void AddBlock(ObjectManager* objectMgr, int Count);

    void SetRender(bool render) { CD.Render = render; }

    const CHUNK& GetChunkData() const { return CD; }

private:
    void Free()override;

private:
    CHUNK CD;
};
END