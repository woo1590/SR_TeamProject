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
	ChunkMesh* Instance = new ChunkMesh;

	if (FAILED(Instance->Ready_Mesh()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ChunkMesh::Ready_Mesh()
{
	static const D3DVERTEXELEMENT9 kDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};

	if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
		return E_FAIL;


	return S_OK;
}

void ChunkMesh::SetBuffer(std::vector<VTXTEX>& v, std::vector<INDEX32> i)
{
	if (VB) Safe_Release(VB);
	if (IB) Safe_Release(IB);

	Vertices = v;
	Indices = i;

	VertexCnt = v.size();
	IndexCnt = i.size() * 3;

	Device->CreateVertexBuffer(VertexCnt * sizeof(VTXTEX), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &VB, 0);
	void* p = nullptr;
	VB->Lock(0, 0, &p, 0);
	memcpy(p, v.data(), VertexCnt * sizeof(VTXTEX));
	VB->Unlock();

	Device->CreateIndexBuffer(IndexCnt * sizeof(_ulong), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &IB, 0);
	p = nullptr;
	IB->Lock(0, 0, &p, 0);
	memcpy_s(p, IndexCnt * sizeof(_ulong), &Indices[0], IndexCnt * sizeof(_ulong));
	IB->Unlock();

}

void ChunkMesh::Draw()
{
	if (VB || IB) return;

	Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
	Device->SetVertexDeclaration(Decl);
	Device->SetIndices(IB);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}
