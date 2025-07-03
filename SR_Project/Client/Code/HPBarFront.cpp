#include "pch.h"
#include "HPBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"
#include "EngineCore.h"
#include "InputSystem.h"

#include "ObjectManager.h"
#include "InfoComponent.h"
#include "Player.h"

HPBarFront* HPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new HPBarFront(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hpUI      = AddComponent<ProgressBar<PlayerInfo>>();
	
	renderer->SetTexture(L"hpbar_front");
	renderer->SetPivot(UIPivot::Bottom);
	renderer->SetScale(0.35f, 0.32f);
	transform->SetPosition(610.f, 690.f);

	return S_OK;
}
