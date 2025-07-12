#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class Material;
class Mesh;
class ENGINE_DLL SpriteRenderer :
    public RendererComponent
{
private:
	SpriteRenderer(Object* owner, RENDER_ID id,
				   const std::string& name, _uint total, _bool repeat);

    virtual ~SpriteRenderer();

public:
	static SpriteRenderer* Create(Object* owner, RENDER_ID id,
							      const std::string& name, _uint total, _bool repeat);
	HRESULT Ready_Component()override;
	void Update(_float dt)override;
    void Render()override;

	void SetMaterial(std::string& key);
	void SetMesh(const std::string& key);

	_uint GetCurrFrame()const { return currFrame; }
private:
	void Free()override;
	
	Material* mtrl = nullptr;
	Mesh* quadMesh = nullptr;

	std::string spriteName;
	_uint currFrame;
	_uint totalFrame;
	_float timer = 0.f;
	_float speed;
	_bool isRepeat = false;
};

END