#include "EnginePCH.h"
#include "RenderSystem.h"
#include "GraphicDevice.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "ChunkManager.h"
#include "Object.h"
#include "SceneManager.h"
#include "Scene.h"
#include "ObjectManager.h"

//component
#include "UIRenderer.h"
#include "RendererComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"
#include "ObjectComponent.h"
#include "FontComponent.h"


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
	RenderList.resize((int)RENDER_ID::Render_End);
	Camera = nullptr;
	Device = GraphicDevice::GetInstance()->GetDevice();
	Device->AddRef();

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	/*---------------Light Setting---------------------*/
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	Device->SetRenderState(D3DRS_SPECULARENABLE, FALSE);

	/*---------------Blend Setting---------------*/
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	/*---------------Point Sprite Setting---------------*/
	Device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
	Device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);

	Device->SetRenderState(D3DRS_POINTSCALE_A, DWORD(0.f));
	Device->SetRenderState(D3DRS_POINTSCALE_B, DWORD(0.f));
	Device->SetRenderState(D3DRS_POINTSCALE_C, DWORD(1.f));

	if (FAILED(D3DXCreateSprite(Device, &spriteBatch)))
		return E_FAIL;

	return S_OK;
}

void RenderSystem::Render()
{
	if (Camera)
	{
		cachedView = Camera->GetViewMatrix();
		cachedProj = Camera->GetProjMatrix(); 

		PriorityPass();
		NonAlphaPass();
		AlphaPass();

		if(EngineCore::GetInstance()->IsDebugMode())
			DebugPass();
	}

	UIPass();
	
	Reset();
	for (auto& list : RenderList)
		list.clear();

	DebugRender.clear();
}

void RenderSystem::RegisterRenderer(RENDER_ID layer, RendererComponent* renderer)
{
	RenderList[(int)layer].push_back(renderer);
}

void RenderSystem::RegisterCollision(CollisionComponent* collision)
{
	DebugRender.push_back(collision);
}

void RenderSystem::SetCamera(Object* cam)
{
	Camera = cam->GetComponent<CameraComponent>();
}

void RenderSystem::SetUIRenderState(UIRenderType newType)
{
	UIRenderer::SetCurRenderType(newType);
}

void RenderSystem::PriorityPass()
{
	Device->SetRenderState(D3DRS_ZWRITEENABLE, false);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (const auto& r : RenderList[(int)RENDER_ID::Render_Priority])
		r->Render();

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void RenderSystem::NonAlphaPass()
{
	for (const auto& r : RenderList[(int)RENDER_ID::Render_NonAlpha])
		r->Render();
}

void RenderSystem::UIPass()
{
	// 낮은 layer 값 먼저 정렬
	auto& uiList = RenderList[(int)RENDER_ID::Render_UI];
	stable_sort(uiList.begin(), uiList.end(), [](auto* a, auto* b)
		{return static_cast<UIRenderer*>(a)->GetLayer() < static_cast<UIRenderer*>(b)->GetLayer(); });

	_matrix originProj;
	Device->GetTransform(D3DTS_PROJECTION, &originProj);

	float width = (float)WINCX;
	float height = (float)WINCY;
	_matrix ortho;
	D3DXMatrixOrthoOffCenterLH(&ortho, 0, width, height, 0, 0.1f, 1.f);

	_matrix identity;
	D3DXMatrixIdentity(&identity);
	Device->SetTransform(D3DTS_WORLD, &identity);
	Device->SetTransform(D3DTS_VIEW, &identity);
	Device->SetTransform(D3DTS_PROJECTION, &ortho);

	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	spriteBatch->Begin(D3DXSPRITE_ALPHABLEND);

	for (auto* renderer : uiList)
	{
		auto* ui = static_cast<UIRenderer*>(renderer);

		if (ui->GetRenderType() != UIRenderer::GetCurRenderType() &&
			ui->GetRenderType() != UIRenderType::Always)
			continue;

		ui->Render();
	}
	spriteBatch->End();

	for (auto* renderer : uiList)
	{
		auto* ui = static_cast<UIRenderer*>(renderer);
		if (ui->GetRenderType() != UIRenderer::GetCurRenderType() && 
			ui->GetRenderType() != UIRenderType::Always)
			continue;

		ui->RenderFont();
	}

	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Device->SetTransform(D3DTS_PROJECTION, &originProj);
}

void RenderSystem::DebugPass()
{
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	CurrView = Camera->GetViewMatrix();
	CurrProj = Camera->GetProjMatrix();

	Device->SetTransform(D3DTS_VIEW, &CurrView);
	Device->SetTransform(D3DTS_PROJECTION, &CurrProj);

	for (const auto& collision : DebugRender)
		collision->Render();
}

void RenderSystem::AlphaPass()
{
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (const auto& r : RenderList[(int)RENDER_ID::Render_Alpha])
		r->Render();

	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

void RenderSystem::Reset()
{
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetRenderState(D3DRS_STENCILENABLE, false);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, true);
	Device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xF);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void RenderSystem::Render_Begin(D3DXCOLOR color)
{
	Device->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
		color,		
		1.f,	
		0);			

	Device->BeginScene();
}

void RenderSystem::Render_End()
{
	Device->EndScene();

	Device->Present(NULL, NULL, NULL, NULL);
}

void RenderSystem::Free()
{
	Safe_Release(spriteBatch);
	Safe_Release(Device);
}

