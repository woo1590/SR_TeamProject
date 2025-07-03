#pragma once

#include "ObjectComponent.h"
#include "UIRenderer.h"

BEGIN(Engine)

class ENGINE_DLL ButtonComponent : public ObjectComponent
{
private:
	explicit ButtonComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static ButtonComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void SetRenderer(UIRenderer* _highlight) { highlight = _highlight; }
	void SetOnClick(function<void()> _callback) { onClick = move(_callback); }

	void Update(float dt) override;

private:
	UIRenderer* highlight = nullptr;
	function<void()> onClick;
	bool isHovered = false;
};

END