#pragma once

BEGIN(Engine)

class ENGINE_DLL Material : public Base
{
private:
    Material();
    virtual ~Material();

public:
    static Material* Create();
    HRESULT Ready_Material();
    HRESULT SetMaterial(D3DMATERIAL9& mtrl);
    HRESULT SetTexture(const std::wstring& filePath, TEXTURE texType);
    void Apply();

private:
    void Free()override;

    D3DMATERIAL9 Mtrl{};
    LPDIRECT3DBASETEXTURE9 Texture = nullptr;

    LPDIRECT3DDEVICE9 Device = nullptr;
};

END