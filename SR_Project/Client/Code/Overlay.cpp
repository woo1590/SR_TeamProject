#include "pch.h"
#include "Overlay.h"
#include "OverlayRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"

Overlay::Overlay(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

Overlay::~Overlay()
{
}

Overlay* Overlay::Create(ObjectManager* owner, ObjectType objType)
{
	Overlay* Instance = new Overlay(owner, objType);
	
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT Overlay::Ready_Object()
{
	auto renderer = AddComponent<OverlayRenderer>(RENDER_ID::Render_Overlay);
	renderer->SetShader("OverlayShader");

	return S_OK;
}

void Overlay::Update(_float dt)
{
	Object::Update(dt);

	if (duration)
	{
		if (timer >= duration)
			isFinish = true;
		
		_float t = timer / duration;
		t = std::clamp(t, 0.f, 1.f);

		GetComponent<OverlayRenderer>()->SetAlpha(alpha);

		if (fadeIn)
		{
			alpha = 1 - t;
		}
		else
		{
			alpha = t;
		}
		GetComponent<OverlayRenderer>()->SetAlpha(alpha);

		timer += dt;
	}
}

void Overlay::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void Overlay::Free()
{
	Object::Free();
}
