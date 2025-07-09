#include "EnginePCH.h"
#include "AnimatorComponent.h"
#include "UIRenderer.h"
#include "Object.h"

AnimatorComponent* AnimatorComponent::Create(Object* owner)
{
	auto* instance = new AnimatorComponent(owner);
	
	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT AnimatorComponent::Ready_Component()
{
	renderer = owner->GetComponent<UIRenderer>();
	assert(renderer && "AnimatorComponent needs UIRenderer");
	return S_OK;
}

void AnimatorComponent::SetFrameSize(int width, int height)
{
	frameWidth = width;
	frameHeight = height;
}

void AnimatorComponent::Play(int start, int end, float speed)
{
	startFrame = start;
	endFrame = end;
	frameSpeed = speed;
	curFrame = start; 
	SetFrame(curFrame);
}

void AnimatorComponent::SetFrame(int idx)
{
	if (!renderer || idx >= totalFrames) return;

	RECT rect;
	rect.left = idx * frameWidth;
	rect.top = 0;
	rect.right = rect.left + frameWidth; 
	rect.bottom = frameHeight;
	
	renderer->SetSrcRect(rect);
}

void AnimatorComponent::Update(float dt)
{
	if (frameWidth == 0 || frameHeight == 0 || totalFrames == 0 || !renderer) return;
	
	frameTimer += dt;
	if (frameTimer >= frameSpeed) 
	{
		frameTimer = 0.f;
		curFrame++;
		if (curFrame > endFrame)
			curFrame = startFrame;

		SetFrame(curFrame);
	}
}
