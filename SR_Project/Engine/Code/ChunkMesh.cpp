#include "EnginePCH.h"
#include "ChunkMesh.h"

ChunkMesh::ChunkMesh()
{
}

ChunkMesh::~ChunkMesh()
{
}

ChunkMesh* ChunkMesh::Create()
{
    ChunkMesh* instance = new ChunkMesh();
    return instance;
}

HRESULT ChunkMesh::Ready_Mesh(const std::vector<VTXTEX>& vertices, const std::vector<uint32_t>& indices)
{
    Safe_Release(VB);
    Safe_Release(IB);

    return CreateBuffer(vertices, indices);
}

HRESULT ChunkMesh::CreateBuffer(const std::vector<VTXTEX>& vertices, const std::vector<uint32_t>& indices)
{
    VertexCnt = static_cast<_ulong>(vertices.size());
    IndexCnt = static_cast<_ulong>(indices.size());

    /*----------Vertex Decl-----------------------------*/
    static const D3DVERTEXELEMENT9 kDecl[] =
    {
        { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                                        D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                                        D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                                        D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
        return E_FAIL;
    /*--------------------------------------------------*/

    if (VertexCnt == 0 || IndexCnt == 0)
        return E_FAIL;

    if (FAILED(Device->CreateVertexBuffer(VertexCnt * sizeof(VTXTEX),
        D3DUSAGE_WRITEONLY,
        0,
        D3DPOOL_MANAGED,
        &VB, nullptr)))
        return E_FAIL;

    void* vtx = nullptr;
    if (FAILED(VB->Lock(0, 0, &vtx, 0)))
        return E_FAIL;

    memcpy_s(vtx, VertexCnt * sizeof(VTXTEX), vertices.data(), VertexCnt * sizeof(VTXTEX));
    VB->Unlock();

    if (FAILED(Device->CreateIndexBuffer(IndexCnt * sizeof(_ulong),
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX32,
        D3DPOOL_MANAGED,
        &IB, nullptr)))
    {
        Safe_Release(VB);
        return E_FAIL;
    }

    void* idx = nullptr;
    if (FAILED(IB->Lock(0, 0, &idx, 0)))
        return E_FAIL;
    memcpy_s(idx, IndexCnt * sizeof(_ulong), indices.data(), IndexCnt * sizeof(_ulong));
    IB->Unlock();

    return S_OK;
}

void ChunkMesh::Draw()
{
    if (!VB || !IB)
        return;

    Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
    Device->SetIndices(IB);
    Device->SetVertexDeclaration(Decl);

    Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void ChunkMesh::Free()
{
    Safe_Release(VB);
    Safe_Release(IB);
    Safe_Release(Decl);
    Mesh::Free();
}