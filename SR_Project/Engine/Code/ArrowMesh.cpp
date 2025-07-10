#include "EnginePCH.h"
#include "ArrowMesh.h"

ArrowMesh::ArrowMesh()
{
}

ArrowMesh::~ArrowMesh()
{
}

ArrowMesh* ArrowMesh::Create()
{
	ArrowMesh* Instance = new ArrowMesh;

	if (FAILED(Instance->Ready_Mesh()))
	{
		Safe_Release(Instance);
		Instance = nullptr;
	}

	return Instance;
}

HRESULT ArrowMesh::Ready_Mesh()
{
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -11.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -15.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -13.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -9.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -7.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -5.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, -3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, -3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, -3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, -3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -25.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -25.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -25.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -25.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -23.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, 3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, 3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, -1.000f, 3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -17.000f, 1.000f, 3.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -21.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -19.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, -3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, -3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, -1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -3.000f, 3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, 3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 3.000f, 3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ -1.000f, 3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 1.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 1.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 3.000f, -1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));
    Vertices.push_back(VTXCUBE({ 1.000f, 3.000f, 1.000f }, { 0.f, 1.f, 0.f }, { 0.f, 0.f, 0.f }));

    Indices.push_back(INDEX32(0, 1, 3));
    Indices.push_back(INDEX32(0, 3, 2));
    Indices.push_back(INDEX32(2, 3, 7));
    Indices.push_back(INDEX32(2, 7, 6));
    Indices.push_back(INDEX32(6, 7, 5));
    Indices.push_back(INDEX32(6, 5, 4));
    Indices.push_back(INDEX32(4, 5, 1));
    Indices.push_back(INDEX32(4, 1, 0));
    Indices.push_back(INDEX32(2, 6, 4));
    Indices.push_back(INDEX32(2, 4, 0));
    Indices.push_back(INDEX32(7, 3, 1));
    Indices.push_back(INDEX32(7, 1, 5));
    Indices.push_back(INDEX32(8, 9, 11));
    Indices.push_back(INDEX32(8, 11, 10));
    Indices.push_back(INDEX32(10, 11, 15));
    Indices.push_back(INDEX32(10, 15, 14));
    Indices.push_back(INDEX32(14, 15, 13));
    Indices.push_back(INDEX32(14, 13, 12));
    Indices.push_back(INDEX32(12, 13, 9));
    Indices.push_back(INDEX32(12, 9, 8));
    Indices.push_back(INDEX32(10, 14, 12));
    Indices.push_back(INDEX32(10, 12, 8));
    Indices.push_back(INDEX32(15, 11, 9));
    Indices.push_back(INDEX32(15, 9, 13));
    Indices.push_back(INDEX32(16, 17, 19));
    Indices.push_back(INDEX32(16, 19, 18));
    Indices.push_back(INDEX32(18, 19, 23));
    Indices.push_back(INDEX32(18, 23, 22));
    Indices.push_back(INDEX32(22, 23, 21));
    Indices.push_back(INDEX32(22, 21, 20));
    Indices.push_back(INDEX32(20, 21, 17));
    Indices.push_back(INDEX32(20, 17, 16));
    Indices.push_back(INDEX32(18, 22, 20));
    Indices.push_back(INDEX32(18, 20, 16));
    Indices.push_back(INDEX32(23, 19, 17));
    Indices.push_back(INDEX32(23, 17, 21));
    Indices.push_back(INDEX32(24, 25, 27));
    Indices.push_back(INDEX32(24, 27, 26));
    Indices.push_back(INDEX32(26, 27, 31));
    Indices.push_back(INDEX32(26, 31, 30));
    Indices.push_back(INDEX32(30, 31, 29));
    Indices.push_back(INDEX32(30, 29, 28));
    Indices.push_back(INDEX32(28, 29, 25));
    Indices.push_back(INDEX32(28, 25, 24));
    Indices.push_back(INDEX32(26, 30, 28));
    Indices.push_back(INDEX32(26, 28, 24));
    Indices.push_back(INDEX32(31, 27, 25));
    Indices.push_back(INDEX32(31, 25, 29));
    Indices.push_back(INDEX32(32, 33, 35));
    Indices.push_back(INDEX32(32, 35, 34));
    Indices.push_back(INDEX32(34, 35, 39));
    Indices.push_back(INDEX32(34, 39, 38));
    Indices.push_back(INDEX32(38, 39, 37));
    Indices.push_back(INDEX32(38, 37, 36));
    Indices.push_back(INDEX32(36, 37, 33));
    Indices.push_back(INDEX32(36, 33, 32));
    Indices.push_back(INDEX32(34, 38, 36));
    Indices.push_back(INDEX32(34, 36, 32));
    Indices.push_back(INDEX32(39, 35, 33));
    Indices.push_back(INDEX32(39, 33, 37));
    Indices.push_back(INDEX32(40, 41, 43));
    Indices.push_back(INDEX32(40, 43, 42));
    Indices.push_back(INDEX32(42, 43, 47));
    Indices.push_back(INDEX32(42, 47, 46));
    Indices.push_back(INDEX32(46, 47, 45));
    Indices.push_back(INDEX32(46, 45, 44));
    Indices.push_back(INDEX32(44, 45, 41));
    Indices.push_back(INDEX32(44, 41, 40));
    Indices.push_back(INDEX32(42, 46, 44));
    Indices.push_back(INDEX32(42, 44, 40));
    Indices.push_back(INDEX32(47, 43, 41));
    Indices.push_back(INDEX32(47, 41, 45));
    Indices.push_back(INDEX32(48, 49, 51));
    Indices.push_back(INDEX32(48, 51, 50));
    Indices.push_back(INDEX32(50, 51, 55));
    Indices.push_back(INDEX32(50, 55, 54));
    Indices.push_back(INDEX32(54, 55, 53));
    Indices.push_back(INDEX32(54, 53, 52));
    Indices.push_back(INDEX32(52, 53, 49));
    Indices.push_back(INDEX32(52, 49, 48));
    Indices.push_back(INDEX32(50, 54, 52));
    Indices.push_back(INDEX32(50, 52, 48));
    Indices.push_back(INDEX32(55, 51, 49));
    Indices.push_back(INDEX32(55, 49, 53));
    Indices.push_back(INDEX32(56, 57, 59));
    Indices.push_back(INDEX32(56, 59, 58));
    Indices.push_back(INDEX32(58, 59, 63));
    Indices.push_back(INDEX32(58, 63, 62));
    Indices.push_back(INDEX32(62, 63, 61));
    Indices.push_back(INDEX32(62, 61, 60));
    Indices.push_back(INDEX32(60, 61, 57));
    Indices.push_back(INDEX32(60, 57, 56));
    Indices.push_back(INDEX32(58, 62, 60));
    Indices.push_back(INDEX32(58, 60, 56));
    Indices.push_back(INDEX32(63, 59, 57));
    Indices.push_back(INDEX32(63, 57, 61));
    Indices.push_back(INDEX32(64, 65, 67));
    Indices.push_back(INDEX32(64, 67, 66));
    Indices.push_back(INDEX32(66, 67, 71));
    Indices.push_back(INDEX32(66, 71, 70));
    Indices.push_back(INDEX32(70, 71, 69));
    Indices.push_back(INDEX32(70, 69, 68));
    Indices.push_back(INDEX32(68, 69, 65));
    Indices.push_back(INDEX32(68, 65, 64));
    Indices.push_back(INDEX32(66, 70, 68));
    Indices.push_back(INDEX32(66, 68, 64));
    Indices.push_back(INDEX32(71, 67, 65));
    Indices.push_back(INDEX32(71, 65, 69));
    Indices.push_back(INDEX32(72, 73, 75));
    Indices.push_back(INDEX32(72, 75, 74));
    Indices.push_back(INDEX32(74, 75, 79));
    Indices.push_back(INDEX32(74, 79, 78));
    Indices.push_back(INDEX32(78, 79, 77));
    Indices.push_back(INDEX32(78, 77, 76));
    Indices.push_back(INDEX32(76, 77, 73));
    Indices.push_back(INDEX32(76, 73, 72));
    Indices.push_back(INDEX32(74, 78, 76));
    Indices.push_back(INDEX32(74, 76, 72));
    Indices.push_back(INDEX32(79, 75, 73));
    Indices.push_back(INDEX32(79, 73, 77));
    Indices.push_back(INDEX32(80, 81, 83));
    Indices.push_back(INDEX32(80, 83, 82));
    Indices.push_back(INDEX32(82, 83, 87));
    Indices.push_back(INDEX32(82, 87, 86));
    Indices.push_back(INDEX32(86, 87, 85));
    Indices.push_back(INDEX32(86, 85, 84));
    Indices.push_back(INDEX32(84, 85, 81));
    Indices.push_back(INDEX32(84, 81, 80));
    Indices.push_back(INDEX32(82, 86, 84));
    Indices.push_back(INDEX32(82, 84, 80));
    Indices.push_back(INDEX32(87, 83, 81));
    Indices.push_back(INDEX32(87, 81, 85));
    Indices.push_back(INDEX32(88, 89, 91));
    Indices.push_back(INDEX32(88, 91, 90));
    Indices.push_back(INDEX32(90, 91, 95));
    Indices.push_back(INDEX32(90, 95, 94));
    Indices.push_back(INDEX32(94, 95, 93));
    Indices.push_back(INDEX32(94, 93, 92));
    Indices.push_back(INDEX32(92, 93, 89));
    Indices.push_back(INDEX32(92, 89, 88));
    Indices.push_back(INDEX32(90, 94, 92));
    Indices.push_back(INDEX32(90, 92, 88));
    Indices.push_back(INDEX32(95, 91, 89));
    Indices.push_back(INDEX32(95, 89, 93));
    Indices.push_back(INDEX32(96, 97, 99));
    Indices.push_back(INDEX32(96, 99, 98));
    Indices.push_back(INDEX32(98, 99, 103));
    Indices.push_back(INDEX32(98, 103, 102));
    Indices.push_back(INDEX32(102, 103, 101));
    Indices.push_back(INDEX32(102, 101, 100));
    Indices.push_back(INDEX32(100, 101, 97));
    Indices.push_back(INDEX32(100, 97, 96));
    Indices.push_back(INDEX32(98, 102, 100));
    Indices.push_back(INDEX32(98, 100, 96));
    Indices.push_back(INDEX32(103, 99, 97));
    Indices.push_back(INDEX32(103, 97, 101));
    Indices.push_back(INDEX32(104, 105, 107));
    Indices.push_back(INDEX32(104, 107, 106));
    Indices.push_back(INDEX32(106, 107, 111));
    Indices.push_back(INDEX32(106, 111, 110));
    Indices.push_back(INDEX32(110, 111, 109));
    Indices.push_back(INDEX32(110, 109, 108));
    Indices.push_back(INDEX32(108, 109, 105));
    Indices.push_back(INDEX32(108, 105, 104));
    Indices.push_back(INDEX32(106, 110, 108));
    Indices.push_back(INDEX32(106, 108, 104));
    Indices.push_back(INDEX32(111, 107, 105));
    Indices.push_back(INDEX32(111, 105, 109));
    Indices.push_back(INDEX32(112, 113, 115));
    Indices.push_back(INDEX32(112, 115, 114));
    Indices.push_back(INDEX32(114, 115, 119));
    Indices.push_back(INDEX32(114, 119, 118));
    Indices.push_back(INDEX32(118, 119, 117));
    Indices.push_back(INDEX32(118, 117, 116));
    Indices.push_back(INDEX32(116, 117, 113));
    Indices.push_back(INDEX32(116, 113, 112));
    Indices.push_back(INDEX32(114, 118, 116));
    Indices.push_back(INDEX32(114, 116, 112));
    Indices.push_back(INDEX32(119, 115, 113));
    Indices.push_back(INDEX32(119, 113, 117));
    Indices.push_back(INDEX32(120, 121, 123));
    Indices.push_back(INDEX32(120, 123, 122));
    Indices.push_back(INDEX32(122, 123, 127));
    Indices.push_back(INDEX32(122, 127, 126));
    Indices.push_back(INDEX32(126, 127, 125));
    Indices.push_back(INDEX32(126, 125, 124));
    Indices.push_back(INDEX32(124, 125, 121));
    Indices.push_back(INDEX32(124, 121, 120));
    Indices.push_back(INDEX32(122, 126, 124));
    Indices.push_back(INDEX32(122, 124, 120));
    Indices.push_back(INDEX32(127, 123, 121));
    Indices.push_back(INDEX32(127, 121, 125));
    Indices.push_back(INDEX32(128, 129, 131));
    Indices.push_back(INDEX32(128, 131, 130));
    Indices.push_back(INDEX32(130, 131, 135));
    Indices.push_back(INDEX32(130, 135, 134));
    Indices.push_back(INDEX32(134, 135, 133));
    Indices.push_back(INDEX32(134, 133, 132));
    Indices.push_back(INDEX32(132, 133, 129));
    Indices.push_back(INDEX32(132, 129, 128));
    Indices.push_back(INDEX32(130, 134, 132));
    Indices.push_back(INDEX32(130, 132, 128));
    Indices.push_back(INDEX32(135, 131, 129));
    Indices.push_back(INDEX32(135, 129, 133));
    Indices.push_back(INDEX32(136, 137, 139));
    Indices.push_back(INDEX32(136, 139, 138));
    Indices.push_back(INDEX32(138, 139, 143));
    Indices.push_back(INDEX32(138, 143, 142));
    Indices.push_back(INDEX32(142, 143, 141));
    Indices.push_back(INDEX32(142, 141, 140));
    Indices.push_back(INDEX32(140, 141, 137));
    Indices.push_back(INDEX32(140, 137, 136));
    Indices.push_back(INDEX32(138, 142, 140));
    Indices.push_back(INDEX32(138, 140, 136));
    Indices.push_back(INDEX32(143, 139, 137));
    Indices.push_back(INDEX32(143, 137, 141));
    Indices.push_back(INDEX32(144, 145, 147));
    Indices.push_back(INDEX32(144, 147, 146));
    Indices.push_back(INDEX32(146, 147, 151));
    Indices.push_back(INDEX32(146, 151, 150));
    Indices.push_back(INDEX32(150, 151, 149));
    Indices.push_back(INDEX32(150, 149, 148));
    Indices.push_back(INDEX32(148, 149, 145));
    Indices.push_back(INDEX32(148, 145, 144));
    Indices.push_back(INDEX32(146, 150, 148));
    Indices.push_back(INDEX32(146, 148, 144));
    Indices.push_back(INDEX32(151, 147, 145));
    Indices.push_back(INDEX32(151, 145, 149));
    Indices.push_back(INDEX32(152, 153, 155));
    Indices.push_back(INDEX32(152, 155, 154));
    Indices.push_back(INDEX32(154, 155, 159));
    Indices.push_back(INDEX32(154, 159, 158));
    Indices.push_back(INDEX32(158, 159, 157));
    Indices.push_back(INDEX32(158, 157, 156));
    Indices.push_back(INDEX32(156, 157, 153));
    Indices.push_back(INDEX32(156, 153, 152));
    Indices.push_back(INDEX32(154, 158, 156));
    Indices.push_back(INDEX32(154, 156, 152));
    Indices.push_back(INDEX32(159, 155, 153));
    Indices.push_back(INDEX32(159, 153, 157));
    Indices.push_back(INDEX32(160, 161, 163));
    Indices.push_back(INDEX32(160, 163, 162));
    Indices.push_back(INDEX32(162, 163, 167));
    Indices.push_back(INDEX32(162, 167, 166));
    Indices.push_back(INDEX32(166, 167, 165));
    Indices.push_back(INDEX32(166, 165, 164));
    Indices.push_back(INDEX32(164, 165, 161));
    Indices.push_back(INDEX32(164, 161, 160));
    Indices.push_back(INDEX32(162, 166, 164));
    Indices.push_back(INDEX32(162, 164, 160));
    Indices.push_back(INDEX32(167, 163, 161));
    Indices.push_back(INDEX32(167, 161, 165));


	if (FAILED(CreateBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT ArrowMesh::CreateBuffer()
{
    VertexCnt = Vertices.size();
    IndexCnt = Indices.size() * 3;

    static const D3DVERTEXELEMENT9 kDecl[] = {
        { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
        D3DDECL_END()
    };

    if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
        return E_FAIL;

    // UV = 위치 기반 임시 대입
    for (auto& vertex : Vertices)
        vertex.vTexUV = vertex.vPosition;

    // 노멀 초기화
    for (auto& vertex : Vertices)
        vertex.vNormal = { 0.f, 0.f, 0.f };

    // 노멀 계산
    for (auto& tri : Indices) {
        _vec3 p0 = Vertices[tri._0].vPosition;
        _vec3 p1 = Vertices[tri._1].vPosition;
        _vec3 p2 = Vertices[tri._2].vPosition;

        _vec3 u = p1 - p0;
        _vec3 v = p2 - p0;
        _vec3 normal;

        D3DXVec3Normalize(&u, &u);
        D3DXVec3Normalize(&v, &v);
        D3DXVec3Cross(&normal, &u, &v);
        D3DXVec3Normalize(&normal, &normal);

        Vertices[tri._0].vNormal += normal;
        Vertices[tri._1].vNormal += normal;
        Vertices[tri._2].vNormal += normal;
    }

    for (auto& vertex : Vertices)
        D3DXVec3Normalize(&vertex.vNormal, &vertex.vNormal);

    // Vertex Buffer 생성
    Device->CreateVertexBuffer(VertexCnt * sizeof(VTXCUBE),
        D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &VB, 0);

    VTXCUBE* pVerts = nullptr;
    VB->Lock(0, 0, (void**)&pVerts, 0);
    memcpy_s(pVerts, VertexCnt * sizeof(VTXCUBE), &Vertices[0], VertexCnt * sizeof(VTXCUBE));
    VB->Unlock();

    // Index Buffer 생성
    Device->CreateIndexBuffer(IndexCnt * sizeof(_ulong),
        D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &IB, 0);

    INDEX32* pIndices = nullptr;
    IB->Lock(0, 0, (void**)&pIndices, 0);
    memcpy_s(pIndices, IndexCnt * sizeof(_ulong), &Indices[0], IndexCnt * sizeof(_ulong));
    IB->Unlock();

    return S_OK;
}

void ArrowMesh::Draw()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXCUBE));
	Device->SetIndices(IB);
	Device->SetVertexDeclaration(Decl);
	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void ArrowMesh::Free()
{
	Safe_Release(VB);
	Safe_Release(IB);
	Safe_Release(Decl);
	Mesh::Free();
}
