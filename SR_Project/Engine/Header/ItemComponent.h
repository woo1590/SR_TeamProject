#pragma once

#include "ObjectComponent.h"
#include "InfoComponent.h"
#include "Object.h"

BEGIN(Engine)
class QuestSystem;

class ENGINE_DLL ItemComponent : public ObjectComponent
{
private:
	explicit ItemComponent(Object* owner) :ObjectComponent(owner) {}

public:
	static ItemComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void SetItemType(ItemType _type);
	ItemType GetItemType() const { return itemType; }
	const ItemInfo& GetItemInfo() const { return owner->GetComponent<InfoComponent<ItemInfo>>()->GetInfo(); }

	void Use(Object* user);

	void SetEquipCallBack(function<void(Object* user)> cb) { onEquipCallback = move(cb); }
	void SetUnEquipCallBack(function<void(Object* user)> cb) { unEquipCallback = move(cb); }

	void Equip(Object* user, optional<int> slotIdx = nullopt);
	void UnEquip(Object* user);

	void SetOriginalScale(const _vec2& _scale) { originalScale = _scale; }
	const _vec2& GetOriginalScale() const { return originalScale; }

	void SetCoolDown(bool _isCool, float _dur);
	bool IsCoolDown() const { return coolDownTimer > 0.f; }
	float GetCoolDownRatio() const;

	void SetUseCallBack(function<void(Object*)> cb) { onUseCallBack = move(cb); }
	optional<int> GetEquippedSlotIdx() const { return equippedSlotIdx; }

	void Update(float dt) override;

private:
	InfoComponent<ItemInfo>* itemInfo = nullptr;
	ItemType itemType = ItemType::Potion;
	function<void(Object* user)> onEquipCallback;
	function<void(Object* user)> unEquipCallback;
	_vec2 originalScale = {1.f, 1.f};
	bool isEquipped = false;
	optional<int> equippedSlotIdx;

	bool isCoolDownItem = false;
	float coolDownTimer = 0.f;
	float coolDownDur = 1.f;

	function<void(Object*)> onUseCallBack;
};

END