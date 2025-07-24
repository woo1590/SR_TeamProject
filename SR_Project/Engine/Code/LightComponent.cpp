#include "EnginePCH.h"
#include "LightComponent.h"
#include "EngineCore.h"

//system
#include "LightSystem.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

LightComponent::LightComponent(Object* owner)
	:ObjectComponent(owner)
{
}

LightComponent::~LightComponent()
{
}

LightComponent* LightComponent::Create(Object* owner)
{
	LightComponent* Instance = new LightComponent(owner);

	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

void LightComponent::Late_Update(_float dt)
{
	_vec3 pos = owner->GetComponent<TransformComponent>()->GetPosition();

	lightInfo.position = pos;
}

void LightComponent::SetLightInfo(float range, _vec3 color)
{
}

bool LightComponent::IsEnabled() const
{
	return isEnable;
}

void LightComponent::SetEnabled(bool enable)
{
	isEnable = enable;
}

void LightComponent::Free()
{

}
