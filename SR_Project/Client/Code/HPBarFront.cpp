#include "pch.h"
#include "HPBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"
#include "HoverComponent.h"
#include "FontComponent.h"
#include "ObjectManager.h"
#include "DamageText.h"


#include "EngineCore.h"

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
	auto hover     = AddComponent<HoverComponent>();
	auto font      = AddComponent<FontComponent>();
	
	renderer->SetTexture(L"hpbar_front");
	renderer->SetPivot(UIPivot::Bottom);
	renderer->SetScale(0.35f, 0.32f);
	transform->SetPosition(610.f, 690.f);

	hover->SetUpdateCallBack([this](bool isHovered) {
		const auto& input = EngineCore::GetInstance()->GetInputSystem();
		auto font = GetComponent<FontComponent>();
		font->ClearText();

		if (!isHovered) return;

		});

	return S_OK;
}

void HPBarFront::Update(float dt)
{
	Object::Update(dt);

	auto playerInfo = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();

	int curHp = playerInfo->GetInfo().curHp;

	if (prevHp >= 0 && curHp < prevHp)
		OnHPChanged(prevHp - curHp);

	prevHp = curHp;
}

void HPBarFront::OnHPChanged(int damage)
{
	auto dmgText = DamageText::Create(owner);
	if (!dmgText) return;

	auto tf = GetComponent<TransformComponent>();
	_vec3 pos = tf->GetWorldPosition();
	_vec3 scale = tf->GetScale();

	pos.x -= 250.f;
	pos.z -= 150.f;
	pos.y += 250.f;

	dmgText->worldPos = pos;

	dmgText->screenDir = _vec2(0.5f,0.5f);

	auto font = dmgText->GetComponent<FontComponent>();
	font->ClearText();
	RECT rc = {0, 0, 200, 80};
	font->AddText(L"-" + to_wstring(damage), rc,
		Color::White,
		DT_CENTER | DT_VCENTER,
		FontType::DmgText);

	owner->AddUIObject(dmgText);
}