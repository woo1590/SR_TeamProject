#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class UIRenderer;

class ENGINE_DLL SlotComponent : public ObjectComponent
{
private:
	explicit SlotComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static SlotComponent* Create(Object* owner) { return new SlotComponent(owner); }

	void SetSlotType(SlotType type) { slotType = type; }
	SlotType GetSlotType() const { return slotType; }

	void SetSlotSize(SlotSize size);
	SlotSize GetSlotSize() const { return slotSize; }

	void OnClick();
	void BindRenderers(UIRenderer* _base, UIRenderer* _highlight);

	void Update(float dt) override;

	void OnHoverEnter();
	void OnHoverExit();

private:
	void ApplySlotSize();

private:
	SlotType slotType = SlotType::Inventory;
	SlotSize slotSize = SlotSize::Default;

	UIRenderer* base = nullptr;      // base slot textrue
	UIRenderer* highlight = nullptr; // hovered texture

	bool isHovered = false;
	bool isSelected = false;

	// ------ effect ---------
	float shakeDuration = 0.f;
	float shakeTime = 0.f;
	float shakePower = 4.f;
	bool  isShaking = false;
	float originalY = 0.f;
};

END