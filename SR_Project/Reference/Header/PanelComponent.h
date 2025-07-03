#pragma once

#include "ObjectComponent.h"

class ENGINE_DLL PanelComponent :public ObjectComponent
{
private:
	explicit PanelComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static PanelComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void SetVisible(bool visible);
	void Toggle();
	bool IsVisible() const { return isVisible; }

	void Update(float dt) override;

private:
	bool isVisible = true;
};

