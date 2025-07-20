#pragma once

#include "RenderTargetView.h"
#include "Object.h"

class InventoryPlayer : public Object
{
private:
	InventoryPlayer(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static InventoryPlayer* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	LPDIRECT3DTEXTURE9 GetMinimapTexture() const { rtv->texture; }
	void UpdateRendererList();
	void Update(float dt) override;
	void Late_Update(float dt) override;

public:
	void Free() override;

private:
	RenderTargetView* rtv = nullptr;
};
