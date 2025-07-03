#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL PanelComponent: public ObjectComponent
{
private:
	explicit PanelComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static PanelComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void Toggle();
	void SetVisible(bool visible);
	bool IsVisible() const;
};

END