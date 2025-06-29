#pragma once

BEGIN(Engine)

class UIObj;

class ENGINE_DLL UIObserver
{
public:
	virtual ~UIObserver() = default;
	virtual void OnClick(UIObj* sender) = 0;
	virtual void OnHover(UIObj* sender) {}
};

END