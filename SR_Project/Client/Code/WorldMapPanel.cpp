#include "pch.h"
#include "WorldMapPanel.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "InputSystem.h"

WorldMapPanel* WorldMapPanel::Create(ObjectManager* owner)
{
	auto instance = new WorldMapPanel(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT WorldMapPanel::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetScale(scale,scale);

	renderer->SetPivot(UIPivot::Center);
	renderer->SetTexture(L"worldmap");
	renderer->SetRenderType(UIRenderType::WorldMap);

	return S_OK;
}

void WorldMapPanel::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	auto transform = GetComponent<TransformComponent>();
	auto renderer = GetComponent<UIRenderer>();

	if (renderer->GetCurRenderType() != renderer->GetRenderType()) return;

	if (input->IsKeyDown(KEY::LBUTTON) && !isDragging)
	{
		isDragging = true;
		dragStartPos = _vec2(input->GetMousePos());
		dragStartOffset = mapOffset;
	}

	if (isDragging && input->IsKeyDown(KEY::LBUTTON))
	{
		_vec2 curPos = _vec2(input->GetMousePos());
		_vec2 delta = curPos - dragStartPos;
		mapOffset = dragStartOffset + delta;
	}

	if (!input->IsKeyDown(KEY::LBUTTON))
		isDragging = false;

	const float edgeSpeed = 200.f;
	const float edgeMargin = 30.f;
	_vec3 mouse = input->GetMousePos();

	if (mouse.x <= edgeMargin)
		mapOffset.x += edgeSpeed * dt;
	else if (mouse.x >= WINCX - edgeMargin)
		mapOffset.x -= edgeSpeed * dt;

	if (mouse.x <= edgeMargin)
		mapOffset.x += edgeSpeed * dt;
	else if (mouse.y >= WINCY - edgeMargin)
		mapOffset.y -= edgeSpeed * dt;

	transform->SetPosition(WINCX * 0.5f + mapOffset.x, WINCY * 0.5f + mapOffset.y);
	
	const float fullWidth = static_cast<float>(renderer->GetFullWidth()) * scale;
	const float fullHeight = static_cast<float>(renderer->GetFullHeight()) * scale;

	const float maxOffsetX = max(0.f, (fullWidth - WINCX) * 0.5f - 20.f);
	const float maxOffsetY = max(0.f, (fullHeight - WINCY) * 0.5f - 20.f);

	mapOffset.x = clamp(mapOffset.x, -maxOffsetX, maxOffsetX);
	mapOffset.y = clamp(mapOffset.y, -maxOffsetY, maxOffsetY);

	transform->SetPosition(WINCX * 0.5f + mapOffset.x, WINCY * 0.5f + mapOffset.y);
}