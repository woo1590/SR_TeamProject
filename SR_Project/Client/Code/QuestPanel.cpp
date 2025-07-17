#include "pch.h"
#include "QuestPanel.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "PanelComponent.h"
#include "UIManager.h"
#include "DialogManager.h"
#include "Scene.h"
#include "EngineCore.h"
#include "InputSystem.h"

QuestPanel* QuestPanel::Create(ObjectManager* owner)
{
	auto* instance = new QuestPanel(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT QuestPanel::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();
	auto panel = AddComponent<PanelComponent>();

	transform->SetPosition(WINCX * 0.5, 600.f);
	
	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetTexture(L"panel");

	return S_OK;
}

void QuestPanel::Update(float dt)
{
	Object::Update(dt);

	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	auto dialog = GetScene()->GetUIManager()->GetDialog();

	static bool wasPressed = false;
	bool nowPressed = input->IsKeyPressed(KEY::LBUTTON);

	if (!wasPressed && nowPressed)
		dialog->SkipOrNext();

	wasPressed = nowPressed;
}