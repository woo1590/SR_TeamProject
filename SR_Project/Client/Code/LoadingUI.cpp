#include "pch.h"
#include "LoadingUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "LoadingStone.h"
#include "ObjectManager.h"

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
	renderer->SetRenderType(UIRenderType::MainGame);
	
	//transform->SetScale(0.65f, 0.4f);
	transform->SetPosition(WINCX * 0.5f, WINCY * 0.5f); 
	renderer->SetTexture(L"loadingscene");

	// -------- tip 주석 -------------------------------------------------------------
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
	// -------------------------------------------------------------------------------------

	tipIndices.resize(tipPool.size());
	iota(tipIndices.begin(), tipIndices.end(), 0);
	random_device random;
	mt19937 g(random());
	shuffle(tipIndices.begin(), tipIndices.end(),g);

	curState = LoadingState::Intro;
	
	return S_OK;
}

void LoadingUI::SetInfo(LOADID nextSceneID)
{ 
	auto renderer = GetComponent<UIRenderer>();
	LoadingInfo info;

	switch (nextSceneID)
	{
	case LOADID::Village:
		if (isFirstLoadToVillage)
		{
			nextStateAfterIntro  = LoadingState::Static;
			info.texturePath     = L"loadingscene";
			staticMsg            = L"Loading...";
			GetComponent<TransformComponent>()->SetScale(targetScale, targetScale * (0.4f / 0.65f));
			scaleTimer = scaleDur;
			isFirstLoadToVillage = false;
		}
		else
		{
			nextStateAfterIntro = LoadingState::TipLoop;
			info.texturePath    = L"loadingscene_stage0";
			info.sceneName      = L"마을";
			info.sceneTip       = L"마을에서는 주민들과 거래할 수 있습니다.";

			prefixText    = L"다음으로 이동";
			sceneNameText = info.sceneName;
			sceneTipText  = L"Tip: " + info.sceneTip; 
			staticMsg.clear();

			if (!tipIndices.empty())
			{
				random_device random;
				mt19937 g(random());
				shuffle(tipIndices.begin(), tipIndices.end(), g);
				curTipIdx = 0;
			}
		}
		break;

	case LOADID::Stage1:
	case LOADID::Stage2:
	default:
		nextStateAfterIntro = LoadingState::TipLoop;

		if (!tipIndices.empty())
		{
			random_device random;
			mt19937 g(random());
			shuffle(tipIndices.begin(), tipIndices.end(), g);
			curTipIdx = 0;
		}
		if (nextSceneID == LOADID::Stage1)
		{
			info.texturePath = L"loadingscene_stage1";
			info.sceneName   = L"하늘섬";
			info.sceneTip    = L"좀비는 단단한 갑옷을 입고 있습니다";
		}
		else 
		{
			info.texturePath = L"loadingscene_stage2";
			info.sceneName   = L"엔더 월드";
			info.sceneTip    = L"낙하하는 함정을 조심하세요!";
		}
		prefixText    = L"다음으로 이동";
		sceneNameText = info.sceneName;
		sceneTipText  = L"Tip: " + info.sceneTip;
		staticMsg.clear();
		break;
	}
	renderer->SetTexture(info.texturePath);

	if (nextStateAfterIntro != LoadingState::Static)
		owner->AddUIObject(LoadingStone::Create(owner));
}

void LoadingUI::Update_Scaling(float dt)
{
	scaleTimer += dt;
	float t = min(scaleTimer / scaleDur, 1.f);

	float easedT = 1.f - (1.f - t) * (1.f - t);
	float curScale = startScale + easedT * (targetScale - startScale);
	GetComponent<TransformComponent>()->SetScale(curScale, curScale * (0.4f / 0.65f));
}

void LoadingUI::Update_Tiploop(float dt)
{
	if (tipPool.empty()) return; 

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
		GetComponent<UIRenderer>()->SetTexture(tipPool[tipToShow].texturePath);
		sceneTipText = L"Tip: " + tipPool[tipToShow].sceneTip;

		scaleTimer = 0.f;
	}
}

void LoadingUI::Update_Static(float dt)
{
	//pulseTime += dt;
}

void LoadingUI::Render_Text(float dt)
{
	auto font = GetComponent<FontComponent>();
	font->ClearText();

	if (curState == LoadingState::Static)
	{
		pulseTime += dt;
		float alpha = 0.5f + 0.5f * sinf(pulseTime * 1.5f);
		font->AddText(staticMsg, {0, 550, 1280, 720}, D3DXCOLOR(1.f, 1.f, 1.f, alpha),
			DT_CENTER, FontType::QuestTitle);
	}
	else
	{
		font->AddText(prefixText, {240, 545, 800, 600}, Color::White, DT_LEFT | DT_TOP, FontType::DeathCount);
		font->AddText(sceneNameText, {240, 585, 800, 700}, Color::Yellow, DT_LEFT | DT_TOP, FontType::DeathText);
		font->AddText(sceneTipText, {750, 545, 1240, 720}, Color::White, 
			DT_LEFT | DT_TOP | DT_WORDBREAK, FontType::MineCraftFont);
	}
}

void LoadingUI::OnLoadComplete()
{
	if (curState == LoadingState::Static && !isLoadComplete)
	{
		isLoadComplete = true;
		staticMsg = L"계속하려면 아무 키나 누르세요.";
	}
}

void LoadingUI::Update(float dt)
{
	Object::Update(dt);

	if (scaleTimer < scaleDur)
		Update_Scaling(dt);

	switch (curState)
	{
	case LoadingState::Intro:
		if (scaleTimer >= scaleDur)
		{
			curState = nextStateAfterIntro;
			if (curState == LoadingState::TipLoop && !tipIndices.empty())
			{
				size_t firstTipToShow = tipIndices[curTipIdx];
				GetComponent<UIRenderer>()->SetTexture(tipPool[firstTipToShow].texturePath);
				sceneTipText = L"Tip: " + tipPool[firstTipToShow].sceneTip;

				scaleTimer = 0.f;
			}
		}
		break;

	case LoadingState::TipLoop:
		Update_Tiploop(dt);
		break;

	case LoadingState::Static:
		Update_Static(dt);
		break;
	}

	Render_Text(dt);  
}