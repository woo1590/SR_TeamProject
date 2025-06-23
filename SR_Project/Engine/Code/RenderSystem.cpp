#include "RenderSystem.h"
#include "GraphicDevice.h"
#include "EngineCore.h"
#include "InputSystem.h"

//component
#include "RendererComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"

RenderSystem::RenderSystem()
{
}

RenderSystem::~RenderSystem()
{
}

RenderSystem* RenderSystem::Create()
{
	RenderSystem* Instance = new RenderSystem;

	if (FAILED(Instance->Ready_RenderSystem()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT RenderSystem::Ready_RenderSystem()
{
	RenderList.resize(RENDER_END);
	Camera = nullptr;
	Device = GraphicDevice::GetInstance()->GetDevice();
	Device->AddRef();

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/*---------------Light Setting---------------------*/
	Device->SetRenderState(D3DRS_LIGHTING, false);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	Device->SetRenderState(D3DRS_SPECULARENABLE, true);

	/*---------------Blend Setting---------------*/
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	/*---------------Texture Setting-------------------------*/
	Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	return S_OK;
}

void RenderSystem::Render()
{
	if (!Camera) return;

	PriorityPass();
	OpaquePass();
	
	Reset();
	for (auto& list : RenderList)
		list.clear();
}

void RenderSystem::RegisterRenderer(RENDER_ID layer, RendererComponent* renderer)
{
	RenderList[layer].push_back(renderer);
}

void RenderSystem::SetCamera(CameraComponent* cam)
{
	Camera = cam;
}

void RenderSystem::PriorityPass()
{
	_matrix view = Camera->GetViewMatrix();
	_matrix skyboxView = view;
	skyboxView._41 = 0.f;
	skyboxView._42 = 0.f;
	skyboxView._43 = 0.f;

	_matrix proj = Camera->GetProjMatrix();
	Device->SetTransform(D3DTS_VIEW, &skyboxView);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (const auto& r : RenderList[RENDER_PRIORITY])
		r->Render();

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void RenderSystem::OpaquePass()
{
	_matrix view = Camera->GetViewMatrix();
	Device->SetTransform(D3DTS_VIEW, &view);

	for (const auto& r : RenderList[RENDER_NONALPHA])
		r->Render();

}

void RenderSystem::Reset()
{
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	Device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void RenderSystem::Render_Begin(D3DXCOLOR color)
{
	Device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		color,		// 후면 버퍼 텍스처 색상
		1.f,		// z버퍼 초기화 값
		0);			// 스텐실 초기화 값

	Device->BeginScene();
}

void RenderSystem::Render_End()
{
	Device->EndScene();

	Device->Present(NULL, NULL, NULL, NULL);
}

void RenderSystem::Free()
{
	Safe_Release(Device);
}

