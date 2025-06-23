#include "StaticMesh.h"

StaticMesh::StaticMesh()
	:Mesh()
{
}

StaticMesh::~StaticMesh()
{
}

StaticMesh* StaticMesh::Create()
{
	StaticMesh* Instance = new StaticMesh;

	return Instance;
}

HRESULT StaticMesh::Ready_Mesh()
{
	Vertices.clear();
	Indices.clear();

	return S_OK;
}

HRESULT StaticMesh::SetMeshData(std::vector<VTXNOR>& vertices, std::vector<INDEX32>& indices)
{
	Vertices = vertices;
	Indices = indices;

	VertexCnt = Vertices.size();
	IndexCnt = Indices.size() * 3;

	//Normal Calculate
	for (auto& vertex : Vertices)
		vertex.vNormal = { 0.f,0.f,0.f };

	for (auto& tri : Indices)
	{
		_vec3 p0 = Vertices[tri._0].vPosition;
		_vec3 p1 = Vertices[tri._1].vPosition;
		_vec3 p2 = Vertices[tri._2].vPosition;

		_vec3 u = p1 - p0;
		_vec3 v = p2 - p0;

		_vec3 normal;
		D3DXVec3Cross(&normal, &u, &v);
		D3DXVec3Normalize(&normal, &normal);

		Vertices[tri._0].vNormal += normal;
		Vertices[tri._1].vNormal += normal;
		Vertices[tri._2].vNormal += normal;
	}

	for (auto& vertex : Vertices)
		D3DXVec3Normalize(&vertex.vNormal, &vertex.vNormal);
	

	//Vertex Buffer
	Device->CreateVertexBuffer(VertexCnt * sizeof(VTXNOR),
							   D3DUSAGE_WRITEONLY,
							   FVF_NOR,
							   D3DPOOL_MANAGED,
							   &VB, 0);

	VTXNOR* verts = nullptr;
	VB->Lock(0, 0, (void**)&verts, 0);
	memcpy_s(verts, VertexCnt * sizeof(VTXNOR), &Vertices[0], VertexCnt * sizeof(VTXNOR));
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

void StaticMesh::Draw()
{
	Device->SetFVF(FVF_NOR);
	Device->SetStreamSource(0, VB, 0, sizeof(VTXNOR));
	Device->SetIndices(IB);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void StaticMesh::Free()
{
	Safe_Release(VB);
	Safe_Release(IB);

	Mesh::Free();
}
