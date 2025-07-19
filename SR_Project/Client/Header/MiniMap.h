#pragma once

#include "CameraComponent.h"
#include "RenderTargetView.h"
#include "Object.h"

class MiniMap : public Object
{
private:
	MiniMap(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static MiniMap* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	LPDIRECT3DTEXTURE9 GetMinimapTexture() const { rtv->texture; }
	void UpdateRendererList();
	void Update(float dt) override;

private:
	RenderTargetView* rtv = nullptr;
};
