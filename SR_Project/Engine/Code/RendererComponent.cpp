#include "EnginePCH.h"
#include "RendererComponent.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "GraphicDevice.h"

//object
#include "Object.h"

//component
#include "TransformComponent.h"

RendererComponent::RendererComponent(Object* owner, RENDER_ID id)
	:ObjectComponent(owner), Id(id),Device(GraphicDevice::GetInstance()->GetDevice())
{
	Device->AddRef();
}

RendererComponent::~RendererComponent()
{
}

void RendererComponent::Late_Update(float dt)
{
	if (Id == Engine::RENDER_ID::Render_None) return;
	EngineCore::GetInstance()->GetRenderSystem()->RegisterRenderer(Id,this);
}

void RendererComponent::SetRenderID(RENDER_ID id)
{
	Id = id;
}

void RendererComponent::Free()
{

}
