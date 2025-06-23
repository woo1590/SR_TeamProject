#pragma once
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL StaticMesh :
    public Mesh
{
public:
    typedef struct tagSubset
    {
        _uint BaseIndex = 0;
        _uint IndexCnt = 0;
    }SUBSET;
private:
    StaticMesh();
    virtual ~StaticMesh();

public:
    static StaticMesh* Create();
    HRESULT Ready_Mesh();

    HRESULT SetMeshData(std::vector<VTXNOR>& vertices, std::vector<INDEX32>& indices);
    void Draw();

private:
    void Free()override;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    std::vector<VTXNOR> Vertices;
    std::vector<INDEX32> Indices;

    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;
};

END