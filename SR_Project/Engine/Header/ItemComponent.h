#pragma once

#include "ObjectComponent.h"
#include "InfoComponent.h"

BEGIN(Engine)

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
	void Equip(Object* use);
	void Drop();

private:
	InfoComponent<ItemInfo>* itemInfo = nullptr;
	ItemType itemType = ItemType::Potion;
};

END