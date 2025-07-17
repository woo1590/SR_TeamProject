#pragma once

#include "ObjectComponent.h"
#include "InfoComponent.h"

BEGIN(Engine)

class QuestSystem;

class ENGINE_DLL ItemComponent : public ObjectComponent
{
private:
	explicit ItemComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static ItemComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void SetItemType(ItemType _type) { itemType = _type; }
	ItemType GetItemType() const { return itemType; }
	void Use(Object* user);

	void SetEquipCallBack(function<void(Object* user)> cb) { onEquipCallback = move(cb); }
	void SetUnEquipCallBack(function<void(Object* user)> cb) { unEquipCallback = move(cb); }

	void Equip(Object* user);
	void UnEquip(Object* user) { if (unEquipCallback) unEquipCallback(user); }

	void SetOriginalScale(const _vec2& _scale) { originalScale = _scale; }
	const _vec2& GetOriginalScale() const { return originalScale; }

private:
	void Drop();

private:
	InfoComponent<ItemInfo>* itemInfo = nullptr;
	ItemType itemType = ItemType::Potion;
	function<void(Object* user)> onEquipCallback;
	function<void(Object* user)> unEquipCallback;
	_vec2 originalScale = {1.f, 1.f};
};

END