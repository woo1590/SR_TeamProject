#pragma once
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CubeMesh :
    public Mesh
{
private:
    CubeMesh();
    virtual ~CubeMesh();

public:
    static CubeMesh* Create();
    HRESULT Ready_Mesh();
    HRESULT CreateBuffer();
    void Draw()override;

private:
    void Free()override;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;   
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    std::vector<VTXCUBE> Vertices;
    std::vector<INDEX32> Indices;

    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;
};

END