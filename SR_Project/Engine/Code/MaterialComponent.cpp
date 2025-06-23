#include "MaterialComponent.h"
#include "GraphicDevice.h"

MaterialComponent::MaterialComponent(Object* owner)
	:ObjectComponent(owner), Device(GraphicDevice::GetInstance()->GetDevice())
{
	Device->AddRef();
}

MaterialComponent::~MaterialComponent()
{
}

MaterialComponent* MaterialComponent::Create(Object* owner)
{
	MaterialComponent* Instance = new MaterialComponent(owner);

	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT MaterialComponent::Ready_Component()
{
	D3DMATERIAL9 defaultMaterial{};
	defaultMaterial.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	defaultMaterial.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	defaultMaterial.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	defaultMaterial.Power = 1.f;

	Materials.push_back(defaultMaterial);

	return S_OK;
}

void MaterialComponent::Apply(_ulong subset)
{
	Device->SetMaterial(&Materials[subset]);
	if (Textures.size() && Textures[subset])
		Device->SetTexture(0, Textures[subset]);
	else
		Device->SetTexture(0, nullptr);
}

void MaterialComponent::SetMaterial(std::vector<D3DMATERIAL9>& materials)
{
	Materials = materials;
}

void MaterialComponent::SetTexture(std::vector<LPDIRECT3DBASETEXTURE9>& textures)
{
	Textures = textures;
}

void MaterialComponent::SetTexture(LPDIRECT3DBASETEXTURE9 texture)
{
	Textures.push_back(texture);
}

void MaterialComponent::Free()
{
	for (auto& tex : Textures)
	{
		Safe_Release(tex);
	}

	Device->Release();
}
