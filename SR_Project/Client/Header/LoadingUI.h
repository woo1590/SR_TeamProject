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
	
private:
	bool mainAnimFinished = false;
	float tipLoopTimer = 0.f;

	static constexpr float TIP_DURATION = 2.5f;
	static constexpr float scaleDur = 2.5f;

	vector<TipAndTexture> tipPool;
	vector<size_t> tipIndices;
	size_t curTipIdx = 0;
	
	float pulseTime = 0.f;

	float scaleTimer = 0.f;
	static constexpr float startScale = 0.65f;
	static constexpr float targetScale = 0.8f;
	

	wstring prefixText;
	wstring sceneNameText;
	wstring sceneTipText;
};