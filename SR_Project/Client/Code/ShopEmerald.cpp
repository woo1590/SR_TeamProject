#include "pch.h"
#include "ShopEmerald.h"
#include "UIRenderer.h"
#include "TransformComponent.h"

ShopEmerald* ShopEmerald::Create(ObjectManager* owner)
{
	auto* instance = new ShopEmerald(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ShopEmerald::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	transform->SetScale(0.2f, 0.2f);
	transform->SetPosition(150.f, 100.f);

	renderer->SetTexture(L"shop_emerald");
	renderer->SetRenderType(UIRenderType::Shop);

	return S_OK;
}
