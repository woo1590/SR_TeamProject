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
#include "UIManager.h"
#include "ResourceManager.h"
#include "Shader.h"

//component
#include "UIRenderer.h"
#include "RendererComponent.h"
#include "CameraComponent.h"
#include "MeshRendererComponent.h"
#include "CollisionComponent.h"
#include "ObjectComponent.h"
#include "FontComponent.h"
#include "TransformComponent.h"


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

	/*---------Init RenderState-----------*/
	{
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

	}

	/*---------Init Surfaces=============*/
	{
		D3DVIEWPORT9 vp;
		Device->GetViewport(&vp);

		if (FAILED(Device->CreateTexture(vp.Width, vp.Height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &targetTexture, nullptr)))
			return E_FAIL;

		if (FAILED(targetTexture->GetSurfaceLevel(0, &targetSurface)))
			return E_FAIL;

		if (FAILED(Device->GetRenderTarget(0, &originSurface)))
			return E_FAIL;

		CreatePostProcessBuffer(vp);
	}
	if (FAILED(D3DXCreateSprite(Device, &spriteBatch)))
		return E_FAIL;

	// ------ Minimap ------------------


	return S_OK;
}

void RenderSystem::Render()
{
	RenderOffScreenViews();
	if (Camera)
	{
		cachedView = Camera->GetViewMatrix();
		cachedProj = Camera->GetProjMatrix(); 

		Device->SetRenderTarget(0, targetSurface);
		Device->Clear(0,
			NULL,
			D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
			D3DXCOLOR(0.f,0.f,1.f,1.f),
			1.f,
			0);

		PriorityPass();
		NonAlphaPass();
		AlphaPass();
		
		if(EngineCore::GetInstance()->IsDebugMode())
			DebugPass();

		Device->SetRenderTarget(0, originSurface);
	}

	PostProcessPass();
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

void RenderSystem::ClearSystem()
{
	for (auto& list : RenderList)
	{
		list.clear();
	}
	Camera = nullptr;
}

void RenderSystem::SetShader(const std::string& key)
{
	postProcessShader = EngineCore::GetInstance()->GetResourceManager()->GetShader(key);
}

HRESULT RenderSystem::CreatePostProcessBuffer(D3DVIEWPORT9 vp)
{
	static const D3DVERTEXELEMENT9 kDecl[] =
	{
		{ 0 , 0 , D3DDECLTYPE_FLOAT4 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITIONT , 0 },
		{ 0 , 16 , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 },
		D3DDECL_END()
	};

	if(FAILED(Device->CreateVertexDeclaration(kDecl, &decl)))
		return E_FAIL;

	std::vector<VTXPP> vertices;
	vertices.push_back({ { -0.5f,vp.Height - 0.5f,0.f,1.f }, { 0.f,1.f } });
	vertices.push_back({ { -0.5f,-0.5f,0.f,1.f }, { 0.f,0.f } });
	vertices.push_back({ { vp.Width-0.5f,-0.5f,0.f,1.f }, { 1.f,0.f } });
	vertices.push_back({ { vp.Width - 0.5f,vp.Height - 0.5f,0.f,1.f }, { 1.f,1.f } });

	Device->CreateVertexBuffer(4 * sizeof(VTXPP), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &postProcessVB, nullptr);

	VTXPP* verts = nullptr;
	postProcessVB->Lock(0, 0, (void**)&verts, 0);
	memcpy_s(verts, 4 * sizeof(VTXPP), vertices.data(), 4 * sizeof(VTXPP));
	postProcessVB->Unlock();

	std::vector<INDEX32> indices;
	indices.push_back({ 0,1,2 });
	indices.push_back({ 0,2,3 });

	Device->CreateIndexBuffer(2 * sizeof(INDEX32), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &postProcessIB, nullptr);

	INDEX32* inds = nullptr;
	postProcessIB->Lock(0, 0, (void**)&inds, 0);
	memcpy_s(inds, 2 * sizeof(INDEX32), indices.data(), 2 * sizeof(INDEX32));
	postProcessIB->Unlock();

	return S_OK;
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

	Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	uiRenderState = UIRenderer::GetCurRenderType();

	const auto& canRender = [this](UIRenderer* renderer)
		{
			const auto type = renderer->GetRenderType();
			return type == uiRenderState || type == UIRenderType::Always;
		};

	spriteBatch->Begin(D3DXSPRITE_ALPHABLEND);

	for (auto* renderer : uiList)
	{
		auto ui = static_cast<UIRenderer*>(renderer);
		if (!canRender(ui)) continue;
		ui->Render(); 
	}
	
	spriteBatch->End();

	for (auto* renderer : uiList)
	{
		auto ui = static_cast<UIRenderer*>(renderer);
		if (!canRender(ui)) continue;
		ui->RenderFont();
	}
	
	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

void RenderSystem::RenderOffScreenViews()
{
	if (rtvs.empty()) return;

	DWORD oldCullMode;
	Device->GetRenderState(D3DRS_CULLMODE, &oldCullMode);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	LPDIRECT3DSURFACE9 oldRenderTarget = nullptr;
	Device->GetRenderTarget(0, &oldRenderTarget);

	_matrix oldView, oldProj;
	Device->GetTransform(D3DTS_VIEW, &oldView);
	Device->GetTransform(D3DTS_PROJECTION, &oldProj);

	for (auto view : rtvs)
	{
		if (!view || !view->camera || !view->surface || view->renderers.empty()) continue;
		Device->SetRenderTarget(0, view->surface);
		Device->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, view->clearColor, 1.f, 0);
		
		Device->BeginScene();

		_matrix viewMatrix = view->camera->GetViewMatrix();
		_matrix projMatrix = view->camera->GetProjMatrix();

		Device->SetTransform(D3DTS_VIEW, & viewMatrix);
		Device->SetTransform(D3DTS_PROJECTION, &projMatrix);

		for (const auto& renderer : view->renderers)
			if (renderer)
			{
				_matrix worldMat = renderer->GetOwner()->GetComponent<TransformComponent>()->GetWorldMatrix();
				Device->SetTransform(D3DTS_WORLD, &worldMat);

				// Pass 1: 앞면 그리기
				Device->SetRenderState(D3DRS_LIGHTING, TRUE);
				Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				renderer->Render();

				// Pass 2: 뒷면 그리기 
				Device->SetRenderState(D3DRS_LIGHTING, FALSE); 
				Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);  
				renderer->Render();
			}

		Device->EndScene();
	}
	
	Device->SetRenderTarget(0, oldRenderTarget);
	Device->SetTransform(D3DTS_VIEW, &oldView);
	Device->SetTransform(D3DTS_PROJECTION, &oldProj);
	Device->SetRenderState(D3DRS_CULLMODE, oldCullMode);
	Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	Safe_Release(oldRenderTarget);
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

	Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
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

void RenderSystem::PostProcessPass()
{
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Device->SetRenderState(D3DRS_ZENABLE, FALSE);

	//for (int i = 0; i < numPass; ++i)
	//{
		postProcessShader->Begin(0);

		postProcessShader->SetTexture("AlbedoMap", targetTexture);

		Device->SetVertexDeclaration(decl);
		Device->SetStreamSource(0, postProcessVB, 0, sizeof(VTXPP));
		Device->SetIndices(postProcessIB);

		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		postProcessShader->End();
	//}

	Device->SetRenderState(D3DRS_ZENABLE, TRUE);
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
	Safe_Release(targetSurface);
	Safe_Release(targetTexture);
	Safe_Release(originSurface);

	Safe_Release(spriteBatch);
	Safe_Release(Device);
}

