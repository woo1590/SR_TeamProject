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

	void SetRenderer(UIRenderer* _r) { renderer = _r; }
	void SetTextures(const wstring& _baseKey, const wstring& _hoverKey) { baseKey = _baseKey; hoverKey = _hoverKey; }

	void SetOnClick(function<void()> _callback) { onClick = move(_callback); }
	void Update(float dt) override;

private:
	void ApplyHover(bool over);

private:
	UIRenderer* renderer = nullptr;
	function<void()> onClick;
	wstring baseKey, hoverKey;
	bool isHovered = false;
	bool prevHover = false;
};

END