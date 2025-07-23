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
	transform->SetPosition(250.f, 420.f);

	renderer->SetRenderType(UIRenderType::QuestUI);
	renderer->SetTexture(L"6");

	return S_OK;
}

void Atri::SetEmotion(Emotion emotion)
{
	auto renderer = GetComponent<UIRenderer>();
	if (!renderer) return;

	switch (emotion)
	{
	case Emotion::p0:  renderer->SetTexture(L"0"); break;
	case Emotion::p1:  renderer->SetTexture(L"1");  break;
	case Emotion::p2:  renderer->SetTexture(L"2");  break;
	case Emotion::p3:  renderer->SetTexture(L"3");  break;
	case Emotion::p4:  renderer->SetTexture(L"4");  break;
	case Emotion::p5:  renderer->SetTexture(L"5");  break;
	case Emotion::p6:  renderer->SetTexture(L"6");  break;
	case Emotion::p7:  renderer->SetTexture(L"7");  break;
	case Emotion::p8:  renderer->SetTexture(L"8");  break;
	case Emotion::p9:  renderer->SetTexture(L"9");  break;
	case Emotion::p10: renderer->SetTexture(L"10"); break;
	case Emotion::p11: renderer->SetTexture(L"11"); break;
	case Emotion::p12: renderer->SetTexture(L"12"); break;
	case Emotion::p13: renderer->SetTexture(L"13"); break;
	case Emotion::p14: renderer->SetTexture(L"14"); break;
	case Emotion::p15: renderer->SetTexture(L"15"); break;
	case Emotion::p16: renderer->SetTexture(L"16"); break;
	case Emotion::p17: renderer->SetTexture(L"17"); break;
	case Emotion::p18: renderer->SetTexture(L"18"); break;
	case Emotion::p19: renderer->SetTexture(L"19"); break;
	case Emotion::p20: renderer->SetTexture(L"20"); break;
	case Emotion::p21: renderer->SetTexture(L"21"); break;
	case Emotion::p22: renderer->SetTexture(L"22"); break;
	case Emotion::p23: renderer->SetTexture(L"23"); break;
	}
}