#include "TerrainMesh.h"
#include "MyMath.h"

TerrainMesh::TerrainMesh()
	:Mesh()
{
}

TerrainMesh::~TerrainMesh()
{
}

TerrainMesh* TerrainMesh::Create()
{
	TerrainMesh* Instance = new TerrainMesh;

	if (FAILED(Instance->Ready_TerrainMesh()))
	{
		Safe_Release(Instance);

		Instance = nullptr;	
	}

	return Instance;
}

HRESULT TerrainMesh::Ready_TerrainMesh()
{
	HeightMap.clear();

	return S_OK;
}

void TerrainMesh::Draw()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
	Device->SetIndices(IB);
	Device->SetFVF(FVF_TEX);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, CellCnt * 2);
}

HRESULT TerrainMesh::LoadTerrain(const std::wstring& filePath, _float cellSpacing, _float heightScale)
{
	HANDLE file = CreateFileW(filePath.c_str(),
						      GENERIC_READ,
						      0,
						      NULL,
						      OPEN_EXISTING,
						      FILE_ATTRIBUTE_NORMAL,
						      0);

	BITMAPFILEHEADER FH{};
	BITMAPINFOHEADER IH{};
	_ulong Byte = 0;

	if (!ReadFile(file, &FH, sizeof(BITMAPFILEHEADER), &Byte, NULL))
		return E_FAIL;

	if (!ReadFile(file, &IH, sizeof(BITMAPINFOHEADER), &Byte, NULL))
		return E_FAIL;

	/*---------Terrain Info Init----------*/
	VertexCol = IH.biHeight;
	VertexRow = IH.biWidth;

	CellCol = VertexCol - 1;
	CellRow = VertexRow - 1;

	VertexCnt = VertexCol * VertexRow;
	CellCnt = CellCol * CellRow;

	CellSpacing = cellSpacing;
	HeightScale = heightScale;

	Width = CellRow * CellSpacing;
	Depth = CellCol * CellSpacing;

	HeightMap.resize(VertexCnt);
	/*-------------------------------------*/

	std::vector<_ulong> pixel(VertexCnt);
	if (!ReadFile(file, &pixel[0], sizeof(_ulong) * VertexCnt, &Byte, NULL))
		return E_FAIL;

	for (_uint i = 0; i < VertexCnt; ++i)
	{
		HeightMap[i] = pixel[i] & 0xff;
	}

	if (FAILED(CreateBuffer()))
		return E_FAIL;

	return S_OK;
}

_float TerrainMesh::GetHeight(_float x, _float z)
{
	x += (Width * 0.5f);
	z = (Depth * 0.5f) - z;

	x /= CellSpacing;
	z /= CellSpacing;

	if (x < 0.f || z < 0.f)
	{
		return -1.f;
	}

	_float col = floorf(x);
	_float row = floorf(z);

	_float A = GetHeightmapEntry(row, col);
	_float B = GetHeightmapEntry(row, col + 1);
	_float C = GetHeightmapEntry(row + 1, col);
	_float D = GetHeightmapEntry(row + 1, col + 1);

	_float dx = x - col;
	_float dz = z - row;
	_float height = 0.f;

	if (dz < 1.f - dx)
	{
		_float uy = B - A;
		_float vy = C - A;

		height = A + math::Lerp(0.f, uy, dx) + math::Lerp(0.f, vy, dz);
	}
	else
	{
		_float uy = B - D;
		_float vy = C - D;

		height = D + math::Lerp(0.f, uy, dx) + math::Lerp(0.f, vy, dz);
	}

	return height*HeightScale;
}

HRESULT TerrainMesh::CreateBuffer()
{
	VTXTEX* vertices;

	Device->CreateVertexBuffer(VertexCnt * sizeof(VTXTEX),
							   D3DUSAGE_WRITEONLY,
							   FVF_TEX,
							   D3DPOOL_MANAGED,
							   &VB, 0);

	VB->Lock(0, 0, (void**)&vertices, 0);

	_uint index = 0;

	_float CoordZ = (Depth * 0.5f);
	_float CoordX = 0.f;

	_float uCoordIncrease = 1.f / CellCol;
	_float vCoordIncrease = 1.f / CellRow;

	for (_uint i = 0; i < VertexCol; ++i)
	{
		CoordX = -(Width * 0.5f);

		for (_uint j = 0; j < VertexRow; ++j)
		{
			index = i * VertexRow + j;
			CoordX =-(Width*0.5f) + CellSpacing * j;

			vertices[index].vPosition =
			{
				CoordX, HeightMap[index] * HeightScale, CoordZ
			};

			vertices[index].vTexUV =
			{
				j * uCoordIncrease,i * vCoordIncrease
			};
		}
		CoordZ -= CellSpacing;
	}

	VB->Unlock();

	/*---------------------------------------*/

	INDEX32* indices;
	
	Device->CreateIndexBuffer(CellCnt * 2 * sizeof(INDEX32),
							  D3DUSAGE_WRITEONLY,
							  D3DFMT_INDEX32,
							  D3DPOOL_MANAGED,
							  &IB, 0);

	IB->Lock(0, 0, (void**)&indices, 0);

	_uint baseIndex = 0;

	for (_uint i = 0; i < CellCol; ++i)
	{
		for (_uint j = 0; j < CellRow; ++j)
		{
			indices[baseIndex] =
			{
				i * VertexRow + j,
				i * VertexRow + j + 1,
				(i + 1) * VertexRow + j
			};

			indices[baseIndex + 1] =
			{
				(i + 1) * VertexRow + j,
				i * VertexRow + j+1,
				(i + 1) * VertexRow + (j + 1)
			};

			baseIndex += 2;
		}
	}

	IB->Unlock();
	return S_OK;
}

_float TerrainMesh::GetHeightmapEntry(_float row, _float col)
{
	return HeightMap[row * VertexRow + col];
}

void TerrainMesh::Free()
{
}
