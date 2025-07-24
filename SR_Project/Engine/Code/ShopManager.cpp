#include "EnginePCH.h"
#include "ShopManager.h"
#include "InventoryManager.h"
#include "InfoComponent.h"
#include "ObjectManager.h"
#include "Object.h"
#include "UIManager.h"
#include "Scene.h"
#include "SlotComponent.h"
#include "UIRenderer.h"
#include "InventoryComponent.h"
#include "SoundManager.h"
#include "EngineCore.h"
#include "QuestSystem.h"

void ShopManager::RegisterShopSlot(Object* slotObj)
{
	if (auto slotComp = slotObj->GetComponent<SlotComponent>())
		shopSlots.push_back(slotComp);
}

void ShopManager::StockItem(ItemType type, int slotIdx)
{
	if (slotIdx >= shopItems.size() || slotIdx >=shopSlots.size()) return;

	if (!onCreateItem)
	{
		assert(false && "CreateItemCallBack is not bound in ShopManager");
		return;
	}

	shopItems[slotIdx] = type;

	Object* itemObj = onCreateItem(type);
	if (itemObj)
	{
		itemObj->GetComponent<UIRenderer>()->SetRenderType(UIRenderType::Shop);
		ui->GetScene()->GetObjectManager()->AddUIObject(itemObj);
		shopSlots[slotIdx]->SetItem(itemObj);
	}
}

bool ShopManager::BuyItem(int shopSlotIdx)
{
	if (shopSlotIdx >= shopItems.size() || !shopItems[shopSlotIdx].has_value())
		return false;

	InventoryManager* invMgr = ui->GetInventory();
	auto playerInfo = ui->GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();

	if (!invMgr || !playerInfo) return false;

	ItemType itemToBuy = *shopItems[shopSlotIdx];
	const auto& itemData = itemTable.at(itemToBuy);
	const auto& itemPrice = (static_cast<int>(itemData.type) + 1);

	if (playerInfo->GetInfo().gold < itemPrice)
	{
		// 골드 부족 메세지
		return false;
	}
	if (!invMgr->FindFirstEmptySlot())
	{
		// 인벤토리 가득참
		return false;
	}
	playerInfo->AddGold(-itemPrice);
	invComp->Add(itemToBuy);
	if (itemToBuy == ItemType::PigItem)
		ui->GetQuestSystem()->ReportQuestProgress(QuestType::BuyPig, 1);
	EngineCore::GetInstance()->GetSoundManager()->PlaySFX("BuyItem");


	auto targetSlot = shopSlots[shopSlotIdx];
	auto itemObject = targetSlot->GetItem(); 

	if (itemObject)
		itemObject->GetComponent<UIRenderer>()->SetVisible(false);

	targetSlot->ClearItem(); 

	shopItems[shopSlotIdx].reset();

	return true;
}

bool ShopManager::BuySelectedItem()
{
	if (!selected) return false;

	auto it = find(shopSlots.begin(), shopSlots.end(), selected);

	if (it == shopSlots.end()) return false;

	int slotIdx = distance(shopSlots.begin(), it);

	return BuyItem(slotIdx);
}

bool ShopManager::SellItem(int invSlotIdx)
{
	auto invMgr = ui->GetInventory();
	auto playerInfo = ui->GetScene()->GetObjectManager()->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>();

	if (!invMgr || !playerInfo) return false;

	ItemType itemToSell;
	if (invMgr->RemoveItemFromSelectedSlot(itemToSell))
	{
		const auto& itemData = itemTable.at(itemToSell);
		const auto& itemPrice = (static_cast<int>(itemData.type) + 1) * 10;
		const int sellPrice = itemPrice / 2;

		playerInfo->AddGold(sellPrice);
		return true;
	}
	return false;
}

void ShopManager::SelectSlot(SlotComponent* newSlot)
{
	if (selected && selected != newSlot)
		selected->Deselect();

	selected = newSlot;
	selected->Select();
}

void ShopManager::DeselectAll()
{
	if (selected)
		selected->Deselect();
	selected = nullptr;
}

void ShopManager::Update(float dt)
{

}