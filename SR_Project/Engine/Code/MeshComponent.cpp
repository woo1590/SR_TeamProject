#include "MeshComponent.h"
#include "GraphicDevice.h"

MeshComponent::MeshComponent(Object* owner)
	:ObjectComponent(owner),Device(GraphicDevice::GetInstance()->GetDevice())
{
	Device->AddRef();
}

MeshComponent::~MeshComponent()
{
}

MeshComponent* MeshComponent::Create(Object* owner)
{
	MeshComponent* Instance = new MeshComponent(owner);

	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

void MeshComponent::SetMesh(const std::vector<VTXNOR>& vertices, const std::vector<INDEX16>& indices)
{
	Vertices = vertices;
	Indices = indices;

	/*-------------법선 벡터 계산----------------*/
	for (auto& vertex : Vertices)
	{
		vertex.vNormal = { 0.f,0.f,0.f };
	}
	
	for (auto& tri : Indices)
	{
		_vec3 v0 = Vertices[tri._0].vPosition;
		_vec3 v1 = Vertices[tri._1].vPosition;
		_vec3 v2 = Vertices[tri._2].vPosition;
	
		_vec3 p0 = v1 - v0;
		_vec3 p1 = v2 - v0;
		_vec3 normal;
		D3DXVec3Cross(&normal, &p0, &p1);
		D3DXVec3Normalize(&normal, &normal);
	
		Vertices[tri._0].vNormal += normal;
		Vertices[tri._1].vNormal += normal;
		Vertices[tri._2].vNormal += normal;	//각 정점에 법선 계속 더해줌
	}
	
	for (auto& vertex : Vertices)
	{
		_vec3 debut = vertex.vNormal;
		D3DXVec3Normalize(&vertex.vNormal, &vertex.vNormal);	//각 정점의 법선 벡터 평균 구하기
	}


	Device = GraphicDevice::GetInstance()->GetDevice();
	/*--------VB 생성---------*/
	Device->CreateVertexBuffer(Vertices.size() * sizeof(VTXNOR),
								D3DUSAGE_WRITEONLY,
								FVF_NOR,
								D3DPOOL_MANAGED,
								&VB, 0);

	VTXCOL* vertexBuffer = nullptr;

	VB->Lock(0, 0, (void**)&vertexBuffer, 0);
	memcpy_s(vertexBuffer, Vertices.size() * sizeof(VTXNOR), Vertices.data(), Vertices.size() * sizeof(VTXNOR));
	VB->Unlock();

	/*--------IB 생성---------*/
	std::vector<_ushort> flatIndices;
	for (const auto& tri : Indices)
	{
		flatIndices.push_back(tri._0);
		flatIndices.push_back(tri._1);
		flatIndices.push_back(tri._2);
	}

	Device->CreateIndexBuffer(flatIndices.size() * sizeof(_ushort),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB, 0);

	_ushort* indexBuffer = nullptr;

	IB->Lock(0, 0, (void**)&indexBuffer, 0);
	std::copy(flatIndices.begin(), flatIndices.end(), indexBuffer);
	IB->Unlock();
}

void MeshComponent::Draw()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXNOR));
	Device->SetIndices(IB);
	Device->SetFVF(FVF_NOR);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, Vertices.size(), 0, Indices.size());
}

void MeshComponent::Free()
{
	VB->Release();
	IB->Release();
	Device->Release();
}
