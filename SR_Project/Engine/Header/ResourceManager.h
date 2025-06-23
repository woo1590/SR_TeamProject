#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class Mesh;
class TerrainMesh;
class Material;
class ENGINE_DLL ResourceManager :
    public Base
{
private:
    ResourceManager();
    virtual ~ResourceManager();

public:
    static ResourceManager* Create();
    HRESULT Ready_ResourceManager();

    void LoadTerrain(const std::wstring& filePath, const std::wstring& key, _float cellSpacing, _float heightScale);
    //직접 지정
    void LoadMesh(const std::wstring& key, Mesh* mesh);
    void LoadMaterial(const std::wstring& key, Material* mtrl);

    Mesh* GetMesh(const std::wstring& key);
    Material* GetMaterial(const std::wstring& key);
    TerrainMesh* GetTerrain(const std::wstring& key);
private:
    void Free()override;

    std::unordered_map<std::wstring, Mesh*> MeshContainer;
    std::unordered_map<std::wstring, Material*> MaterialContainer;
    std::unordered_map<std::wstring, TerrainMesh*> TerrainContainer;
};

END