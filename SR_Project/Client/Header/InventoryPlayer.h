#pragma once

#include "Object.h"
#include "RenderTargetView.h"

class InventoryPlayer : public Object
{
private:
	InventoryPlayer(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static InventoryPlayer* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	LPDIRECT3DTEXTURE9 GetInvTexture() const { return rtv->texture; }

private:
	RenderTargetView* rtv = nullptr;
};
