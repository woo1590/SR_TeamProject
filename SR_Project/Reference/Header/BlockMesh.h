#pragma once
#include "Mesh.h"
#include "Engine_Struct.h"

BEGIN(Engine)
class ENGINE_DLL BlockMesh : public Mesh
{
private:
    BlockMesh();
    virtual ~BlockMesh();

public:
    static BlockMesh* Create();
    HRESULT Ready_Mesh();
    void Draw() override;

private:
    void BuildIBEntry();
    HRESULT CreateBuffer();
    void Free() override;

private:
    struct IBEntry { LPDIRECT3DINDEXBUFFER9 ib; _uint tri; };

    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;

    std::vector<VTXCUBE> Vertices;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    std::vector<IBEntry> IBTable;
    UINT8 Mask;

    LPDIRECT3DVERTEXDECLARATION9 Decl = nullptr;

};
END