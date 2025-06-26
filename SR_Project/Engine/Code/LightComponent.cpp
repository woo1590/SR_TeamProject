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

void LightComponent::SetPointLight(float range, D3DCOLOR color)
{
	Type = LightType::Point;

	D3DCOLORVALUE value{
	value.a = ((color >> 24) & 0xFF) / 255.f,
	value.r = ((color >> 16) & 0xFF) / 255.f,
	value.g = ((color >> 8) & 0xFF) / 255.f,
	value.b = ((color >> 0) & 0xFF) / 255.f
	};

	LightData.Type = D3DLIGHT_POINT;
	LightData.Diffuse = value;
	LightData.Specular = LightData.Diffuse;
	LightData.Ambient = LightData.Diffuse * 0.5f;
	LightData.Position = owner->GetComponent<TransformComponent>()->GetPosition();
	LightData.Range = range;

	Is_Enabled = true;

	EngineCore::GetInstance()->GetLightSystem()->RegisterLight(this);
}

void LightComponent::SetDirectionalLight(_vec3 direction, D3DCOLOR color)
{
	Type = LightType::Directional;

	D3DCOLORVALUE value{
	value.a = ((color >> 24) & 0xFF) / 255.f,
	value.r = ((color >> 16) & 0xFF) / 255.f,
	value.g = ((color >> 8) & 0xFF) / 255.f,
	value.b = ((color >> 0) & 0xFF) / 255.f
	};

	LightData.Type = D3DLIGHT_DIRECTIONAL;
	LightData.Diffuse = value;
	LightData.Specular = LightData.Diffuse * 0.3f;
	LightData.Ambient = LightData.Diffuse * 0.5f;
	LightData.Direction = direction;

	Is_Enabled = true;

	EngineCore::GetInstance()->GetLightSystem()->RegisterLight(this);
}

const D3DLIGHT9& LightComponent::GetLightData() const
{
	return LightData;
}

bool LightComponent::IsEnabled() const
{
	return Is_Enabled;
}

void LightComponent::SetEnabled(bool enable)
{
	Is_Enabled = enable;
}

void LightComponent::Free()
{

}
