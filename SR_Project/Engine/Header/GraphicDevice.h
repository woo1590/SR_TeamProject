#pragma once

BEGIN(Engine)

class ENGINE_DLL GraphicDevice :public Base
{
	DECLARE_SINGLETON(GraphicDevice)
private:
	GraphicDevice();
	~GraphicDevice();

public:
	HRESULT Ready_GraphicDev(HWND hWnd, WINMODE eMode, const _uint width, const _uint height, GraphicDevice** ppGraphicDev);
	LPDIRECT3DDEVICE9 GetDevice()const;

	void Render_Begin(D3DXCOLOR color);
	void Render_End();
private:
	LPDIRECT3D9 SDK = nullptr;		//1¹ø °´Ã¼
	LPDIRECT3DDEVICE9 Device = nullptr;	//3¹ø °´Ã¼

	void Free()override;
};

END