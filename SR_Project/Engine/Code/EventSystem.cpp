#include "EnginePCH.h"
#include "EventSystem.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "CollisionSystem.h"

//component
#include "CameraComponent.h"

EventSystem::EventSystem()
{
}

EventSystem::~EventSystem()
{
}

EventSystem* EventSystem::Create()
{
	EventSystem* Instance = new EventSystem;

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
		HitInfo hit;
		Ray ray = Camera->ScreenPointRay();
		auto collision = EngineCore::GetInstance()->GetCollisionSystem();

		hit = collision->Raycast(ray);

	}

}

void EventSystem::Free()
{
}
