#pragma once

BEGIN(Engine)

class ENGINE_DLL IHoverable
{
public:
	virtual void OnHoverEnter() = 0;
	virtual void OnHoverExit() = 0;
};

END