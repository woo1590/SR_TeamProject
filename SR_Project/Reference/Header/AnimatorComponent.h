#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class UIRenderer;

class ENGINE_DLL AnimatorComponent : public ObjectComponent
{
private:
	AnimatorComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static AnimatorComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void SetFrame(int idx);
	void Play(int start, int end, float speed);

	void SetFrameSize(int width, int height);
	void SetTotalFrameCount(int count) { totalFrames = count; }

	void SetSheetSize(int columns) { sheetColumns = columns > 0 ? columns : 1; }

	void Update(float dt) override;


	int GetCurFrame() const { return curFrame; }
	float GetSpeed() const { return frameSpeed; }

	RECT GetCurRect() const {
		RECT rect;
		rect.left = curFrame * frameWidth;
		rect.top = 0;
		rect.right = rect.left + frameWidth;
		rect.bottom = frameHeight;
		return rect;
	}

private:
	int curFrame = 0;
	int startFrame = 0;
	int endFrame = 0;
	int totalFrames = 0;

	int sheetColumns = 1;

	int frameWidth = 0;
	int frameHeight = 0;

	float frameTimer = 0.f;
	float frameSpeed = 0.1f;

	UIRenderer* renderer = nullptr;
};

END