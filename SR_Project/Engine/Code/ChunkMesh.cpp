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

HRESULT ChunkMesh::Ready_Mesh(const std::vector<VTXTEX>& vertices, const std::vector<int>& indices)
{
    Safe_Release(VB);
    Safe_Release(IB);

    return CreateBuffer(vertices, indices);
}

HRESULT ChunkMesh::CreateBuffer(const std::vector<VTXTEX>& vertices, const std::vector<int>& indices)
{
    VertexCnt = static_cast<_ulong>(vertices.size());
    IndexCnt = static_cast<_ulong>(indices.size());

    if (VertexCnt == 0 || IndexCnt == 0)
        return E_FAIL;

    if (FAILED(Device->CreateVertexBuffer(VertexCnt * sizeof(VTXTEX),
        D3DUSAGE_WRITEONLY,
        FVF_TEX,
        D3DPOOL_MANAGED,
        &VB, nullptr)))
        return E_FAIL;

    void* vtx = nullptr;
    if (FAILED(VB->Lock(0, 0, &vtx, 0)))
        return E_FAIL;

    memcpy_s(vtx, VertexCnt * sizeof(VTXTEX), vertices.data(), VertexCnt * sizeof(VTXTEX));
    VB->Unlock();

    if (FAILED(Device->CreateIndexBuffer(IndexCnt * sizeof(int),
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
    memcpy_s(idx, IndexCnt * sizeof(int), indices.data(), IndexCnt * sizeof(int));
    IB->Unlock();

    return S_OK;
}

void ChunkMesh::Draw()
{
    if (!VB || !IB)
        return;

    Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
    Device->SetIndices(IB);
    Device->SetFVF(FVF_TEX);
    Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void ChunkMesh::Free()
{
    Safe_Release(VB);
    Safe_Release(IB);
    Mesh::Free();
}