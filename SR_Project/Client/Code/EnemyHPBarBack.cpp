#include "pch.h"
#include "EnemyHPBarBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

EnemyHPBarBack* EnemyHPBarBack::Create(ObjectManager* owner)
{
	auto* instance = new EnemyHPBarBack(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT EnemyHPBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();

	renderer->SetTexture(L"enemy_hpbarback");
	renderer->SetRenderType(UIRenderType::MainGame);
	renderer->SetPivot(UIPivot::Left);
	renderer->SetAlpha(0.6f);

	return S_OK;
}