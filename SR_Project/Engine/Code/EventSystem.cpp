#include "EnginePCH.h"
#include "EventSystem.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "CollisionSystem.h"
#include "CameraManager.h"

#include "Scene.h"

//component
#include "CameraComponent.h"

EventSystem::EventSystem(Scene* owner)
	:owner(owner)
{
}

EventSystem::~EventSystem()
{
}

EventSystem* EventSystem::Create(Scene* owner)
{
	EventSystem* Instance = new EventSystem(owner);

	if (FAILED(Instance->Ready_EventSystem()))
	{
		Safe_Release(Instance);

		Instance = nullptr;	
	}

	return Instance;
}

HRESULT EventSystem::Ready_EventSystem()
{

	return S_OK;
}

void EventSystem::Update()
{
	if (Camera)
	{
		auto Input = EngineCore::GetInstance()->GetInputSystem();
		if (Input->IsKeyPressed(LBUTTON))
		{
			HitInfo hit;
			Ray ray = Camera->ScreenPointRay();
			auto collision = owner->GetCollisionSystem();

			hit = collision->Raycast(ray);


		}
	}
}

void EventSystem::SetCamera()
{
	Camera = owner->GetCameraManager()->GetMainCamera();
}

void EventSystem::Free()
{
}
