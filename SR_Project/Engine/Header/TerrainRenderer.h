#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class Mesh;
class Material;
class ENGINE_DLL TerrainRenderer :
    public RendererComponent
{
private:
    TerrainRenderer(Object* owner, RENDER_ID renderId);
    virtual ~TerrainRenderer();

public:
    static TerrainRenderer* Create(Object* owner, RENDER_ID renderId);
    void Render();

    void SetMesh(const std::wstring& key);
    void SetMaterial(const std::wstring& key);
private:
    void Free()override;

    Mesh* mesh = nullptr;
    Material* Mtrl = nullptr;
    LPDIRECT3DDEVICE9 Device = nullptr;
};

END