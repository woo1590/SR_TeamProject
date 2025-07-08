#pragma once
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL ChunkMesh :
    public Mesh
{
private:
    ChunkMesh();
    virtual ~ChunkMesh();

public:
    static ChunkMesh* Create();
    HRESULT Ready_Mesh();
    void SetBuffer(std::vector<VTXTEX>& v, std::vector<INDEX32> i);

    void Draw();
private:
    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    std::vector<VTXTEX> Vertices;
    std::vector<INDEX32> Indices;

    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;

    LPDIRECT3DVERTEXDECLARATION9 Decl = nullptr;
};

END