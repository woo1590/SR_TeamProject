#include "pch.h"
#include "LoadingStone.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "AnimatorComponent.h"
#include "FontComponent.h"

LoadingStone* LoadingStone::Create(ObjectManager* owner)
{
	auto* instance = new LoadingStone(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LoadingStone::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(150.f, 600.f);
	transform->SetScale(0.5f, 0.5f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"loading_stone");
	renderer->SetRenderType(UIRenderType::Always);
	

	auto animator = AddComponent<AnimatorComponent>();
	animator->SetFrameSize(240, 240);
	animator->SetTotalFrameCount(59);
	animator->SetSheetSize(8);
	animator->Play(0, 59, 0.06f); 
	return S_OK;
}
