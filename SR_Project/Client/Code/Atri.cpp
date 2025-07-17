#include "pch.h"
#include "Atri.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

Atri* Atri::Create(ObjectManager* owner)
{
	auto* instance = new Atri(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Atri::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetScale(0.3f, 0.2f);
	transform->SetPosition(280.f, 460.f);

	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetTexture(L"atri");

	return S_OK;
}

void Atri::SetEmotion(const wstring& emotion)
{
	auto renderer = GetComponent<UIRenderer>();
	if (!renderer) return;

	//if (emotion == L"happy")
	//	renderer->SetTexture(L"atri_happy");
	//else if (emotion == L"sad")
	//	renderer->SetTexture(L"atri_sad");
	//else if (emotion == L"angry")
	//	renderer->SetTexture(L"atri_angry");
	//else
	//	renderer->SetTexture(L"atri");
}