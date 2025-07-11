#include "EnginePCH.h"
#include "WorldUIComponent.h"
#include "TransformComponent.h"
#include "Object.h"
#include "EngineCore.h"
#include "GraphicDevice.h"
#include "RenderSystem.h"

WorldUIComponent* WorldUIComponent::Create(Object* owner)
{
	auto instance = new WorldUIComponent(owner);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WorldUIComponent::Ready_Component()
{
	return S_OK;
}

void WorldUIComponent::Update(float dt)
{
    if (!targetTf) return;

    float depthZ = 0.f;
    _vec2 screen = ToScreen(targetTf->GetWorldPosition() + offset, ScreenMode::Position,&depthZ);
    float scale = clamp(1.f / (0.1f + depthZ), minScale, maxScale);

    auto tf = owner->GetComponent<TransformComponent>();
    tf->SetWorldPosition({_vec3(screen.x, screen.y, 0.f)});
    tf->SetScale(baseScaleX * scale, baseScaleY * scale);
}

_vec2 WorldUIComponent::ToScreen(const _vec3& vec, ScreenMode mode, float* outZ)
{
    auto* device = GraphicDevice::GetInstance()->GetDevice();
    D3DVIEWPORT9 viewport;
    device->GetViewport(&viewport);

    const _matrix view = EngineCore::GetInstance()->GetRenderSystem()->GetCachedViewMatrix();
    const _matrix proj = EngineCore::GetInstance()->GetRenderSystem()->GetCachedProjMatrix();

    if (mode == ScreenMode::Position)
    {
        _vec3 screen;
        D3DXVec3Project(&screen, &vec, &viewport, &proj, &view, nullptr);
        if (outZ)
            *outZ = screen.z;
        return {_vec2(screen.x, screen.y)};
    }
    else
    {
        const _vec3 p0(0, 0, 0), p1 = vec;
        _vec3 s0, s1;
        D3DXVec3Project(&s0, &p0, &viewport, &proj, &view, nullptr);
        D3DXVec3Project(&s1, &p1, &viewport, &proj, &view, nullptr);

        _vec2 dir = {_vec2(s1.x - s0.x, s1.y - s0.y)};
        if (D3DXVec2LengthSq(&dir) > 0)
            D3DXVec2Normalize(&dir,&dir);
        return dir;
    }
}
