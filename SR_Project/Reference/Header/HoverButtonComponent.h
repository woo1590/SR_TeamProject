#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)
class UIRenderer;
class HoverComponent;

class ENGINE_DLL HoverButtonComponent : public ObjectComponent
{
private:
	explicit HoverButtonComponent(Object* owner) : ObjectComponent(owner) {}

public:
	static HoverButtonComponent* Create(Object* owner);
	HRESULT Ready_Component();

	void BindRenderers(UIRenderer* _base, UIRenderer* _hover);
	void SetOnClick(function<void()> func){ onClick = move(func); }

	void SetHighlightScale(_vec2 _base, _vec2 _max);
	void SetHoverState(bool on);

	void Update(float dt) override;

private:
	UIRenderer* base = nullptr;
	UIRenderer* hover = nullptr;
	function<void()> onClick;

	bool isHovered = false;
	float curScale = 1.f;
	float scaleSpeed = 4.f;

	_vec2 baseScale = {1.f, 1.f};
	_vec2 maxScale = {1.5f, 1.5f};
};

END