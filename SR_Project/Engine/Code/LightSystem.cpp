#include "EnginePCH.h"
#include "LightSystem.h"
#include "GraphicDevice.h"

//component
#include "LightComponent.h"

LightSystem::LightSystem()
{
}

LightSystem::~LightSystem()
{
}

LightSystem* LightSystem::Create()
{
    LightSystem* Instance = new LightSystem;

    if (FAILED(Instance->Ready_LightSystem()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT LightSystem::Ready_LightSystem()
{
    Lights.clear();

    return S_OK;
}

void LightSystem::RegisterLight(LightComponent* light)
{
    Lights.push_back(light);
}

void LightSystem::UnRegisterLight(LightComponent* light)
{
    std::erase(Lights, light);
}

void LightSystem::ApplyLight()
{
    int index = 0;
    auto device = GraphicDevice::GetInstance()->GetDevice();

    for (auto& light : Lights)
    {
        if (light->IsEnabled())
        {
            

            ++index;
        }
    }
}

void LightSystem::Free()
{
    Lights.clear();
}
