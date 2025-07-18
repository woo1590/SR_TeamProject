#include "pch.h"
#include "LoadingUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

LoadingUI* LoadingUI::Create(ObjectManager* owner)
{
	auto* instance = new LoadingUI(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LoadingUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();
	
	transform->SetScale(0.65f, 0.4f);
	transform->SetPosition(WINCX * 0.5f, WINCY * 0.5f); 
	renderer->SetTexture(L"loadingscene");

    tipPool.push_back({L"loadingscene_tip1",  L"크리퍼는 조용히 다가와 폭발합니다."});
    tipPool.push_back({L"loadingscene_tip2",  L"레드스톤은 복잡한 장치를 만들 수 있습니다."});
    tipPool.push_back({L"loadingscene_tip3",  L"밤에는 몬스터가 나타나니 안전한 곳을 찾으세요."});
    tipPool.push_back({L"loadingscene_tip4",  L"철은 장비 제작의 핵심 재료입니다."});
    tipPool.push_back({L"loadingscene_tip5",  L"엔더맨과 눈을 마주치지 마세요."});
	tipPool.push_back({L"loadingscene_tip6",  L"다이아몬드는 지하 16층 이하에서만 발견됩니다."});
	tipPool.push_back({L"loadingscene_tip7",  L"배고픔 수치가 6 이하가 되면 체력 회복이 멈춥니다."});
	tipPool.push_back({L"loadingscene_tip8",  L"엔더 드래곤을 처치하려면 엔드 포탈을 완성해야 합니다."});
	tipPool.push_back({L"loadingscene_tip9",  L"낚시는 희귀 아이템과 전리품을 얻는 좋은 방법입니다."});
	tipPool.push_back({L"loadingscene_tip10", L"TNT는 강력하지만 주변 환경을 파괴하니 주의하세요."});
	tipPool.push_back({L"loadingscene_tip11", L"에메랄드는 마을 주민과의 거래에 사용됩니다."});

	tipIndices.resize(tipPool.size());
	iota(tipIndices.begin(), tipIndices.end(), 0);
	random_device random;
	mt19937 g(random());
	shuffle(tipIndices.begin(), tipIndices.end(),g);

	return S_OK;
}

void LoadingUI::SetInfo(LOADID nextSceneID)
{ 
	LoadingInfo info;
	switch (nextSceneID)
	{
	case LOADID::Village:
		info.texturePath = L"loadingscene";
		info.sceneName   = L"캠프";
		info.sceneTip    = L"상인에게서 새로운 장비를 구매할 수 있습니다.";
		break;

	case LOADID::Stage1:
		info.texturePath = L"loadingscene_stage1";
		info.sceneName   = L"선인장 협곡";
		info.sceneTip    = L"좀비는 단단한 갑옷을 입고 있습니다";
		break;

	case LOADID::Stage2:
		info.texturePath = L"loadingscene_stage2";
		info.sceneName   = L"레드스톤 광산";
		info.sceneTip    = L"낙하하는 함정을 조심하세요!";
		break;
	}

	GetComponent<UIRenderer>()->SetTexture(info.texturePath);
	
	prefixText = L"다음으로 이동";
	sceneNameText = info.sceneName;
	sceneTipText = L"Tip: " + info.sceneTip;
}

void LoadingUI::Update(float dt)
{
	Object::Update(dt);

	auto transform = GetComponent<TransformComponent>();
	auto font = GetComponent<FontComponent>();
	auto renderer = GetComponent<UIRenderer>();

	if (!mainAnimFinished)
	{
		scaleTimer += dt;
		float t = min(scaleTimer / scaleDur, 1.f);
		float curScale = startScale + t * (targetScale - startScale);
		transform->SetScale(curScale, curScale * (0.4f / 0.65f));

		if (scaleTimer >= scaleDur)
		{
			mainAnimFinished = true;

			size_t FirstTipToShow = tipIndices[curTipIdx];
			renderer->SetTexture(tipPool[FirstTipToShow].texturePath);
			sceneTipText = L"Tip: " + tipPool[FirstTipToShow].sceneTip;

			scaleTimer = 0.f;
		}
	}
	else
	{
		tipLoopTimer += dt;
		if (tipLoopTimer >= TIP_DURATION)
		{
			tipLoopTimer = 0.f;
			curTipIdx++;

			if (curTipIdx >= tipIndices.size())
			{
				curTipIdx = 0;
				random_device random;
				mt19937 g(random());
				shuffle(tipIndices.begin(), tipIndices.end(), g);
			}
			size_t tipToShow = tipIndices[curTipIdx];
			renderer->SetTexture(tipPool[tipToShow].texturePath);
			sceneTipText = L"Tip: " + tipPool[tipToShow].sceneTip;

			scaleTimer = 0.f;
		}
		scaleTimer += dt; 
		float t = min(scaleTimer / scaleDur, 1.f);
		float curScale = startScale + t * (targetScale - startScale);
		transform->SetScale(curScale, curScale * (0.4f / 0.65f));
	}

	pulseTime += dt;
	float alpha = 0.5f + 0.5f * sinf(pulseTime * 2.f);
	
	font->ClearText();

	font->AddText(prefixText, {240, 545, 800, 600}, Color::White, DT_LEFT | DT_TOP, 
		FontType::DeathCount);        

	font->AddText(sceneNameText, {240, 595, 800, 700}, Color::Yellow, DT_LEFT | DT_TOP, 
		FontType::DeathText);       

	font->AddText(sceneTipText, {650, 550, 1100, 700}, Color::White, DT_CENTER | DT_WORDBREAK,
		FontType::MineCraftFont);
}