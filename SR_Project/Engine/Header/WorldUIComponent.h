#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

enum class ScreenMode {Position,Direction};

class TransformComponent;

class ENGINE_DLL WorldUIComponent : public ObjectComponent
{
private:
	explicit WorldUIComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static WorldUIComponent* Create(Object* owner);
	HRESULT Ready_Component();


	void SetTarget(TransformComponent* target) { targetTf = target; }
	void SetOffset(const _vec3 offset) { this->offset = offset; }

	void Update(float dt) override;

	void SetBaseScale(float _scaleX, float _scaleY) { baseScaleX = _scaleX; baseScaleY = _scaleY; }
	void SetScaleRange(float min, float max) { minScale = min; maxScale = max; }

	static _vec2 ToScreen(const _vec3& vec, ScreenMode mode, float* outZ = nullptr);

private:
	TransformComponent* targetTf = nullptr;
	_vec3 offset{};
	float baseScaleX = 1.f, baseScaleY = 1.f;
	float minScale = 0.5f, maxScale = 1.5f;
};

END