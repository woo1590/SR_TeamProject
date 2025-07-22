#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

using HoverCallBack = function<void(bool)>;
using HoverUpdateCallBack = function<void(bool isHovered)>;

class ENGINE_DLL HoverComponent : public ObjectComponent 
{
private:
	explicit HoverComponent(Object* owner):ObjectComponent(owner) {}

public:
	static HoverComponent* Create(Object* owner) { return new HoverComponent(owner); }
	void Update(float dt) override;

	void SetCallBack(HoverCallBack _callback) { callBack = move(_callback); }
	void SetUpdateCallBack(HoverUpdateCallBack _callBack) { updateCallBack = move(_callBack); }
	void SetRightClickCallBack(function<void()> callback) { onRightClick = move(callback); }

	bool IsHovered() const { return isHovered; }

private:
	bool isHovered = false;
	HoverCallBack callBack;
	HoverUpdateCallBack updateCallBack;
	function<void()> onRightClick;
};

END