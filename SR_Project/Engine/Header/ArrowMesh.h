#pragma once
#include "Mesh.h"
struct VERTEX {
    D3DXVECTOR3 vPos;
    D3DXVECTOR3 vNormal;
    D3DXVECTOR2 vUV;
};
#define FVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)

BEGIN(Engine)

class ENGINE_DLL ArrowMesh : public Mesh {
private:
    ArrowMesh();
    virtual ~ArrowMesh();

public:
    static ArrowMesh* Create();
    HRESULT Ready_Mesh();
    HRESULT CreateBuffer();
    void Draw() override;

private:
    void Free() override;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    std::vector<VTXCUBE> Vertices;
    std::vector<INDEX32> Indices;

    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;

    LPDIRECT3DVERTEXDECLARATION9 Decl = nullptr;
};

END