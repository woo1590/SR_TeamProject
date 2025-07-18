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

	transform->SetScale(2.f , 2.f);
	transform->SetPosition(250.f, 430.f);

	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetTexture(L"atri_none");

	return S_OK;
}

void Atri::SetEmotion(Emotion emotion)
{
	auto renderer = GetComponent<UIRenderer>();
	if (!renderer) return;

	switch (emotion)
	{
	case Emotion::None:     renderer->SetTexture(L"atri_none");     break;
	case Emotion::Angry:    renderer->SetTexture(L"atri_angry");    break;
	case Emotion::Sad:      renderer->SetTexture(L"atri_sad");      break;
	case Emotion::Happy:    renderer->SetTexture(L"atri_happy");    break;
	case Emotion::Emm:      renderer->SetTexture(L"atri_emm");      break;
	case Emotion::CloseEye: renderer->SetTexture(L"atri_closeeye"); break;
	case Emotion::Confuse:  renderer->SetTexture(L"atri_confuse");  break;
	case Emotion::Brave:    renderer->SetTexture(L"atri_brave");    break;
	}
}