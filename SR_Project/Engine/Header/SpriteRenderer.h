#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class Shader;
class Mesh;
class ENGINE_DLL SpriteRenderer :
	public RendererComponent
{
private:
	SpriteRenderer(Object* owner,
		const std::string& name, _uint total, _float speed, _float size, _bool repeat);

	virtual ~SpriteRenderer();

public:
	static SpriteRenderer * Create(Object* owner,
							      const std::string& name, _uint total, _float speed, _float size, _bool repeat = false);
	HRESULT Ready_Component()override;
	void Update(_float dt)override;
	void Render()override;

	_uint GetCurrFrame()const { return currFrame; }
	_bool IsSpriteEnd()const { return !isRepeat && isSpriteEnd; }
private:
	void Free()override;
	
	Shader* shader = nullptr;
	Mesh* quadMesh = nullptr;
	_float size = 1.f;

	std::string spriteName;
	_uint currFrame = 0.f;
	_uint totalFrame;
	_float timer = 0.f;
	_float speed = 2.f;
	_bool isRepeat = true;
	_bool isSpriteEnd = false;

};

END