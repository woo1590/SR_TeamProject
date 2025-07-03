#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

using HoverCallBack = function<void(bool)>;

class ENGINE_DLL HoverComponent : public ObjectComponent 
{
private:
	explicit HoverComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static HoverComponent* Create(Object* owner) { return new HoverComponent(owner); }
	void Update(float dt) override;

	void SetCallBack(HoverCallBack _callback) { callBack = move(_callback); }

private:
	bool isHovered = false;
	HoverCallBack callBack;
};

END