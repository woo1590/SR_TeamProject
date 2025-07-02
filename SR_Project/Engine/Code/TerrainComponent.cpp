#include "TerrainComponent.h"
#include "GraphicDevice.h"

//object
#include "Object.h"

//component
#include "MaterialComponent.h"

TerrainComponent::TerrainComponent(Object* owner)
    :ObjectComponent(owner), Device(GraphicDevice::GetInstance()->GetDevice())
{
    Device->AddRef();
}

TerrainComponent::~TerrainComponent()
{
}

TerrainComponent* TerrainComponent::Create(Object* owner)
{
    TerrainComponent* Instance = new TerrainComponent(owner);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);

        Instance = nullptr; 
    }

    return Instance;
}

HRESULT TerrainComponent::CreateTerrain(const std::wstring& filePath, _uint vertsRow, _uint vertsCol, _float spacing, _float scale)
{
    VertsRow = vertsRow;
    VertsCol = vertsCol;
    CellSpacing = spacing;
    HeightScale = scale;
    
    CellsRow = VertsRow - 1;
    CellsCol = VertsCol - 1;
    Width = CellsRow * CellSpacing;
    Depth = CellsCol * CellSpacing;

    VertexCnt = vertsCol * VertsRow;
    TriangleCnt = CellsCol * CellsRow * 2;

    if (FAILED(LoadHeightMapBMP(filePath)))
        return E_FAIL;

    if (FAILED(CreateVertices()))
        return E_FAIL;

    if (FAILED(CreateIndices()))
        return E_FAIL;

    if (FAILED(LoadTexture()))
        return E_FAIL;

    return S_OK;
}

void TerrainComponent::Draw()
{
    auto material = owner->GetComponent<MaterialComponent>();

    Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
    Device->SetIndices(IB);
    Device->SetFVF(FVF_TEX);
    material->Apply(0);

    Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, TriangleCnt);
}

HRESULT TerrainComponent::LoadHeightMap(const std::wstring& filePath)
{
    std::vector<BYTE> data(VertexCnt);

    std::ifstream file(filePath.c_str(), std::ios::binary);
    if (!file.is_open())
        return E_FAIL;

    file.seekg(0, std::ios::end);
    _uint filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    file.read((char*)&data[0],data.size());
    file.close();

    HeightMap.resize(VertexCnt);
    for (_uint i = 0; i < data.size ();++i)
        HeightMap[i] = data[i];

    return S_OK;
}

HRESULT TerrainComponent::LoadHeightMapBMP(const std::wstring& filePath)
{
    FileHandle = CreateFileW(filePath.c_str(),
                             GENERIC_READ,
                             0,
                             NULL, 
                             OPEN_EXISTING, 
                             FILE_ATTRIBUTE_NORMAL,
                             0);

    if (INVALID_HANDLE_VALUE == FileHandle)
        return E_FAIL;

    _ulong byte = 0;

    ReadFile(FileHandle, &FHeader, sizeof(BITMAPFILEHEADER), &byte, NULL);
    ReadFile(FileHandle, &IHeader, sizeof(BITMAPINFOHEADER), &byte, NULL);

    std::vector<_ulong> pixels(VertexCnt);
    HeightMap.resize(VertexCnt);
    ReadFile(FileHandle, &pixels[0], sizeof(_ulong) * VertexCnt, &byte, NULL);

    for (_uint i = 0; i < VertexCnt; ++i)
    {
        HeightMap[i] = pixels[i] & 0xff;
    }

    CloseHandle(FileHandle);

    return S_OK;
}

HRESULT TerrainComponent::CreateVertices()
{
    std::vector<VTXTEX> vertices(VertexCnt);
    Device->CreateVertexBuffer(VertexCnt*sizeof(VTXTEX),
                               D3DUSAGE_WRITEONLY,
                               FVF_TEX,
                               D3DPOOL_MANAGED,
                               &VB,0);

    int startX = -(Width * 0.5f);
    int startZ = Depth * 0.5f;

    int endX = Width * 0.5f;
    int endZ = -(Depth * 0.5f);

    _float uCoordIncrease = 1.f / CellsCol;
    _float vCoordIncrease = 1.f / CellsRow;

    VTXTEX* terrainVertices = nullptr;

    VB->Lock(0, 0, (void**)&terrainVertices, 0);

    int i = 0;
    for (int z = startZ;z >= endZ;z -= CellSpacing)
    {
        int j = 0;
        for (int x = startX; x <= endX; x += CellSpacing)
        {
            int index = i * VertsRow + j;
            
            terrainVertices[index] = VTXTEX(
                { (_float)x,(_float)HeightMap[index]*HeightScale,(_float)z },
                { j * uCoordIncrease, i * vCoordIncrease }
            );
            ++j;
        }
        ++i;
    }

    VB->Unlock();

    return S_OK;
}

HRESULT TerrainComponent::CreateIndices()
{
    Device->CreateIndexBuffer(TriangleCnt * sizeof(INDEX16),
                              D3DUSAGE_WRITEONLY,
                              D3DFMT_INDEX16,
                              D3DPOOL_MANAGED,
                              &IB, 0);

    INDEX16* terrainIndices = nullptr;
    IB->Lock(0, 0, (void**)&terrainIndices, 0);

    int baseIndex = 0;

    for (_uint i = 0; i < CellsCol; ++i)
    {
        for (_uint j = 0; j < CellsRow; ++j)
        {
            terrainIndices[baseIndex] = INDEX16(
                i * VertsRow + j,
                i * VertsRow + j + 1,
                (i + 1) * VertsRow + j
            );
            terrainIndices[baseIndex + 1] = INDEX16(
                (i + 1) * VertsRow + j,
                i * VertsRow + j + 1,
                (i + 1) * VertsRow + j + 1
            );

            baseIndex += 2;
        }
    }

    IB->Unlock();

    return S_OK;
}

HRESULT TerrainComponent::LoadTexture()
{
    LPDIRECT3DBASETEXTURE9 Tex = nullptr;

    HRESULT hr = D3DXCreateTextureFromFile(GraphicDevice::GetInstance()->GetDevice(),
        "../Terrain0.png", (LPDIRECT3DTEXTURE9*)&Tex);

    owner->GetComponent<MaterialComponent>()->SetTexture(Tex);
    
    if (FAILED(hr))
        return E_FAIL;

    return S_OK; 
}

HRESULT TerrainComponent::GenerateTexture()
{
    LPDIRECT3DBASETEXTURE9 Tex = nullptr;
    
    _uint texWidth = CellsRow;
    _uint texHeight = CellsCol;

    D3DXCreateTexture(GraphicDevice::GetInstance()->GetDevice(),
        texWidth, texHeight, 0, 0,
        D3DFMT_A8B8G8R8,
        D3DPOOL_MANAGED,
        (LPDIRECT3DTEXTURE9*)&Tex);

    D3DSURFACE_DESC texDesc;
    ((LPDIRECT3DTEXTURE9)Tex)->GetLevelDesc(0, &texDesc);

    D3DLOCKED_RECT lockRect;
    ((LPDIRECT3DTEXTURE9)Tex)->LockRect(0, &lockRect, 0, 0);

    DWORD* texData = (DWORD*)lockRect.pBits;
    for (_uint y = 0; y < texHeight;++y)
    {
        for (_uint x = 0; x < texWidth; ++x)
        {
            D3DXCOLOR c;

            _float height = GetHeightMap(y, x);

            if (height < 40.f) c = D3DCOLOR_XRGB(255, 249, 157);
            else if (height < 80.f)c = D3DCOLOR_XRGB(124, 197, 118);
            else if (height < 120.f)c = D3DCOLOR_XRGB(0, 166, 81);
            else if (height < 160.f)c = D3DCOLOR_XRGB(25, 123, 48);
            else if (height < 200.f)c = D3DCOLOR_XRGB(115, 100, 87);
            else c = D3DCOLOR_XRGB(255, 255, 255);

            _float cosine = CalculateShade(y, x);

            texData[y * lockRect.Pitch / 4 + x] = c * cosine;
        }
    }

    ((LPDIRECT3DTEXTURE9)Tex)->UnlockRect(0);

    owner->GetComponent<MaterialComponent>()->SetTexture(Tex);

    return S_OK;
}

_float TerrainComponent::CalculateShade(_uint row, _uint col)
{
    _float heightA = GetHeightMap(row, col);
    _float heightB = GetHeightMap(row, col + 1);
    _float heightC = GetHeightMap(row + 1, col);

    _vec3 u{ CellSpacing,heightB - heightA,0.f };   //단순 계산이기 때문에 높이 간의 기울기만 중요함
    _vec3 v{ 0.f,heightC - heightA,CellSpacing };

    _vec3 n;
    D3DXVec3Cross(&n, &v, &u);
    D3DXVec3Normalize(&n, &n);

    _vec3 lightDir{ -1.f,0.f,0.f };
    D3DXVec3Normalize(&lightDir, &lightDir);
    _float cosine = D3DXVec3Dot(&n, &lightDir);

    if (cosine <= 0.f)
        cosine = 0.f;

    return cosine;
}

_float TerrainComponent::GetHeightMap(_uint row, _uint col)
{
    return HeightMap[row * VertsRow + col];
}

void TerrainComponent::Free()
{
    VB->Release();
    IB->Release();
    Device->Release();

    ObjectComponent::Free();
}
