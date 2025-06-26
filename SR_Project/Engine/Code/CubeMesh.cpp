#include "EnginePCH.h"
#include "CubeMesh.h"

CubeMesh::CubeMesh()
{
}

CubeMesh::~CubeMesh()
{
}

CubeMesh* CubeMesh::Create()
{
	CubeMesh* Instance = new CubeMesh;

	return Instance;
}

HRESULT CubeMesh::Ready_Mesh()
{
	return S_OK;
}

HRESULT CubeMesh::SetMeshData(std::vector<VTXCUBE>& vertices, std::vector<INDEX32>& indices)
{
	Vertices = vertices;
	Indices = indices;

	VertexCnt = Vertices.size();
	IndexCnt = Indices.size() * 3;

	for (auto& vertex : Vertices)
	{
		vertex.vTexUV = vertex.vPosition;
	}

	//Vertex Buffer
	Device->CreateVertexBuffer(VertexCnt * sizeof(VTXCUBE),
		D3DUSAGE_WRITEONLY,
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&VB, 0);

	VTXCUBE* verts = nullptr;
	VB->Lock(0, 0, (void**)&verts, 0);
	memcpy_s(verts, VertexCnt * sizeof(VTXCUBE), &Vertices[0], VertexCnt * sizeof(VTXCUBE));
	VB->Unlock();

	//Index Buffer
	Device->CreateIndexBuffer(IndexCnt * sizeof(_ulong),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_MANAGED,
		&IB, 0);

	INDEX32* index = nullptr;
	IB->Lock(0, 0, (void**)&index, 0);
	memcpy_s(index, IndexCnt * sizeof(_ulong), &Indices[0], IndexCnt * sizeof(_ulong));
	IB->Unlock();

	return S_OK;
}

void CubeMesh::Draw()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXCUBE));
	Device->SetIndices(IB);
	Device->SetFVF(FVF_CUBE);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void CubeMesh::Free()
{
	Safe_Release(VB);
	Safe_Release(IB);
	Mesh::Free();
}
