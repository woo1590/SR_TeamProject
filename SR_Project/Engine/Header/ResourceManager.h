#pragma once

BEGIN(Engine)

class Mesh;
class TerrainMesh;
class Material;
class Shader;
class ENGINE_DLL ResourceManager : public Base
{
private:
    ResourceManager();
    virtual ~ResourceManager();

public:
    static ResourceManager* Create();
    HRESULT Ready_ResourceManager();

    /*----------File Load----------------*/
    //void LoadResource(const std::wstring& texPath, const std::wstring& texKey, TEXTURE texType, const std::wstring& mtrlKey);
    //void LoadShader(const std::wstring & filePath,const std::wstring& key);
    //void LoadTexture(const std::wstring& filePath, const std::wstring& key, TEXTURE texType);
    //void LoadMaterial(const std::wstring& key, Material* mtrl);

    /*------------Load .mat File----------------*/
    void LoadMesh(const std::string& key, Mesh* mesh);
    void LoadMaterial(const std::string& filePath);
    Shader* LoadShader(const std::string& key);
    LPDIRECT3DBASETEXTURE9 LoadTexture(const std::string& key, TEXTURE texType);

    Mesh *                 GetMesh(const std::string& key);
    Material*              GetMaterial(const std::string& key);
    Shader*                GetShader(const std::string& key);
    LPDIRECT3DBASETEXTURE9 GetTexture(const std::string& key);
    LPDIRECT3DBASETEXTURE9 GetTexture(const std::wstring& key);

    void RegisterUILayer(const wstring& tag, int layer);
    optional<int> GetUILayer(const wstring& tag) const;

private:
    void Free()override;

    std::unordered_map<std::string, Mesh*>                  MeshContainer;
    std::unordered_map<std::string, Material*>              MaterialContainer;
    std::unordered_map<std::string, LPDIRECT3DBASETEXTURE9> TextureContainer;
    std::unordered_map<std::wstring, LPDIRECT3DBASETEXTURE9> UITextureContainer;
    std::unordered_map<std::string, Shader*>                ShaderContainer;

    unordered_map<wstring, int> uiLayerTable;
};

END