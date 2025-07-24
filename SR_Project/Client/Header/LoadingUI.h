#pragma once

struct LoadingInfo
{
	wstring texturePath;
	wstring sceneName;
	wstring sceneTip;
};

struct TipAndTexture
{
	wstring texturePath;
	wstring sceneTip;
};

enum class LoadingState {Intro, TipLoop,Static};

#include "Object.h"

class LoadingUI : public Object
{
private:
	LoadingUI(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static LoadingUI* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
	
	void SetInfo(LOADID nextSceneID);
	bool IsAnimFinished() const { return scaleTimer >= scaleDur; }

	void Update_Scaling(float dt);
	void Update_Tiploop(float dt);
	void Update_Static(float dt);
	void Render_Text(float dt);

	LoadingState GetCurState() const { return curState; }
	void OnLoadComplete();
	
private:
	bool isLoadComplete = false;
	bool mainAnimFinished = false;
	float tipLoopTimer = 0.f;

	static constexpr float TIP_DURATION = 3.f;
	static constexpr float scaleDur = 3.f;

	vector<TipAndTexture> tipPool;
	vector<size_t> tipIndices;
	size_t curTipIdx = 0;
	
	float pulseTime = 0.f;

	float scaleTimer = 0.f;
	static constexpr float startScale = 0.65f;
	static constexpr float targetScale = 0.75f;
	
	wstring prefixText;
	wstring sceneNameText;
	wstring sceneTipText;

	wstring staticMsg;
	LoadingState curState = LoadingState::Intro;
	LoadingState nextStateAfterIntro = LoadingState::TipLoop;

	inline static bool isFirstLoadToVillage = true;
};