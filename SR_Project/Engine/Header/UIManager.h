#pragma once 

BEGIN(Engine)

class Scene;
class InventoryManager; 
class QuestSystem;
class TooltipManager;

class ENGINE_DLL UIManager: public Base
{
public:
	UIManager(Scene* owner)
		:owner(owner) {}

	static UIManager* Create(Scene* owner);
	HRESULT Ready_UIManager();

	InventoryManager* GetInventory() const { return inventory; }
	QuestSystem* GetQuestSystem() const { return quest; }
	TooltipManager* GetTooltip() const { return tooltip; }

	Scene* GetScene() const { return owner; }

	void Update(float dt);
	void Free() override;

private:
	Scene* owner = nullptr;
	InventoryManager* inventory = nullptr;
	QuestSystem* quest = nullptr;
	TooltipManager* tooltip = nullptr;
};

END