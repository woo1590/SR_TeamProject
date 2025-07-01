#pragma once

BEGIN(Engine)

class Mesh;
class TerrainMesh;
class Material;
class ENGINE_DLL ResourceManager : public Base
{
private:
    ResourceManager();
    virtual ~ResourceManager();

public:
    static ResourceManager* Create();
    HRESULT Ready_ResourceManager();

    /*----------�ܺ� ���� �ε� �Լ�----------------*/
    void LoadTexture(const std::wstring& filePath, const std::wstring& key, TEXTURE texType);

    /*----------���� ���� �Լ�(�׽�Ʈ��)-------------*/
    void LoadMesh(const std::wstring& key, Mesh* mesh);
    void LoadMaterial(const std::wstring& key, Material* mtrl);

    Mesh*           GetMesh(const std::wstring& key);
    Material*       GetMaterial(const std::wstring& key);
    LPDIRECT3DBASETEXTURE9 GetTexture(const std::wstring& key);
    TerrainMesh*    GetTerrain(const std::wstring& key);

    void RegisterUILayer(const wstring& tag, int layer);
    optional<int> GetUILayer(const wstring& tag) const;


private:
    void Free()override;

    std::unordered_map<std::wstring, Mesh*> MeshContainer;
    std::unordered_map<std::wstring, Material*> MaterialContainer;
    std::unordered_map<std::wstring, LPDIRECT3DBASETEXTURE9> TextureContainer;

    unordered_map<wstring, int> uiLayerTable;
};

END