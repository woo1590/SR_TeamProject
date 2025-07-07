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

	void SetItem(Object* obj) { itemObj = obj; }
	void ClearItem() { itemObj = nullptr; }
	bool HasItem() const { return itemObj != nullptr; }

	Object* GetItem()const { return itemObj; }

	void SetAllowedType(SlotItemType type) { allowedType = type; }
	SlotItemType GetAllowedType() const { return allowedType; }

	void OnClick();
	void BindRenderers(UIRenderer* _base, UIRenderer* _highlight);

	void Update(float dt) override;

	void OnHoverEnter() { isHovered = true; }
	void OnHoverExit() { isHovered = false; }

	void Select() { isSelected = true; }
	void Deselect() { isSelected = false; }

private:
	SlotType slotType = SlotType::Inventory;
	SlotItemType allowedType = SlotItemType::Any;

	UIRenderer* base = nullptr;
	UIRenderer* highlight = nullptr; 

	Object* itemObj = nullptr;
	
	bool isHovered = false;
	bool isSelected = false;

	float shakeDuration = 0.f;
	float shakeTime = 0.f;
	float shakePower = 4.f;
	bool  isShaking = false;
	float originalY = 0.f;
};

END