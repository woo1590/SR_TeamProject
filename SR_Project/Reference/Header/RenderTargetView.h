#pragma once

#include "CameraComponent.h"
#include "MeshRendererComponent.h"

BEGIN(Engine)

struct ENGINE_DLL RenderTargetView
{
	CameraComponent* camera = nullptr;
	LPDIRECT3DTEXTURE9 texture = nullptr;
	LPDIRECT3DSURFACE9 surface = nullptr;
	D3DXCOLOR clearColor = D3DCOLOR_ARGB(0, 0, 0,0);

	list<MeshRenderer*> renderers;

	HRESULT CreateTexture(IDirect3DDevice9* device, UINT width, UINT height, D3DFORMAT format)
	{
		if (FAILED(device->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET,
			format, D3DPOOL_DEFAULT, &texture, NULL)))
			return E_FAIL;

		texture->GetSurfaceLevel(0, &surface);
		return S_OK;
	}

	void Release()
	{
		Safe_Release(surface);
		Safe_Release(texture);
	}
};

END