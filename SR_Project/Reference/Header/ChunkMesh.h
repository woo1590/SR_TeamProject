#pragma once
#include "Mesh.h"
#include "Engine_Struct.h"

BEGIN(Engine)
class ENGINE_DLL ChunkMesh : public Mesh
{
private:
    ChunkMesh();
    virtual ~ChunkMesh();

public:
    static ChunkMesh* Create();
    HRESULT Ready_Mesh(const std::vector<VTXTEX>& vertices, const std::vector<int>& indices);
    void Draw() override;

private:
    HRESULT CreateBuffer(const std::vector<VTXTEX>& vertices, const std::vector<int>& indices);
    void Free() override;

private:
    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;
    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;
};
END