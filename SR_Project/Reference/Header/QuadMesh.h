#pragma once
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL QuadMesh :
    public Mesh
{
private:
    QuadMesh();
    virtual ~QuadMesh();

public:
    static QuadMesh* Create();
    HRESULT Ready_Mesh();
    void Draw();
private:
    void Free()override;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    LPDIRECT3DVERTEXDECLARATION9 Decl = nullptr;
};

END