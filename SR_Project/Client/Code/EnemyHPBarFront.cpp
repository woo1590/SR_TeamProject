#include "pch.h"
#include "EnemyHPBarFront.h"
#include "UIRenderer.h"
#include "ProgressBar.h"
#include "GraphicDevice.h"
#include "RenderSystem.h"
#include "EngineCore.h"
#include "FontComponent.h"

EnemyHPBarFront* EnemyHPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new EnemyHPBarFront(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT EnemyHPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hpBar     = AddComponent<ProgressBar<EnemyInfo>>();
	auto font      = AddComponent<FontComponent>();

	hpBar->SetBarDirection(BarDirection::Horizontal);
	hpBar->SetEventType(UIEventType::HP_Changed);
	
	renderer->SetAlpha(0.8f);
	renderer->SetPivot(UIPivot::Left);
	renderer->SetRenderType(UIRenderType::MainGame);
	renderer->SetTexture(L"enemy_hpbarfront");

	return S_OK;
}

void EnemyHPBarFront::Update(float dt)
{
	Object::Update(dt);
	if (!monsterTf) return;

	_vec3 worldPos = monsterTf->GetWorldPosition() + _vec3(-3.f, 2.f, 0.f);

	float depthZ = 0.f;
	_vec2 screen = WorldToScreen(worldPos, depthZ);
	float scale = clamp(1.f / (0.1f + depthZ), 0.5f, 1.5f);

	scale *= 0.2f;

	auto tf = GetComponent<TransformComponent>();
	tf->SetScale(scale, scale);
	tf->SetWorldPosition({_vec3(screen.x, screen.y, 0.f)});
	
	auto font = GetComponent<FontComponent>();
	font->ClearText();
	wstringstream ss;
	ss << L"ScreenPos: (" << (int)screen.x << L", " << (int)screen.y << L") Z: " << depthZ;

	RECT rect = {(LONG)screen.x, (LONG)screen.y - 30, (LONG)screen.x + 200, (LONG)screen.y};
	font->AddText(ss.str(), rect, D3DXCOLOR(1.f, 1.f, 0, 1.f));
}

_vec2 EnemyHPBarFront::WorldToScreen(const _vec3& worldPos, float& outZ)
{
	auto* device = GraphicDevice::GetInstance()->GetDevice();

	D3DVIEWPORT9 viewport;
	device->GetViewport(&viewport);

	const _matrix view = EngineCore::GetInstance()->GetRenderSystem()->GetCachedViewMatrix();
	const _matrix proj = EngineCore::GetInstance()->GetRenderSystem()->GetCachedProjMatrix();

	_vec3 screen;
	D3DXVec3Project(&screen, &worldPos, &viewport, &proj, &view, nullptr);

	outZ = screen.z;
	return _vec2(screen.x, screen.y);
}
