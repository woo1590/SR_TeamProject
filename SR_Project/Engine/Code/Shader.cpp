#include "EnginePCH.h"
#include "Shader.h"
#include "GraphicDevice.h"

Shader::Shader(LPD3DXEFFECT effect)
	:Device(GraphicDevice::GetInstance()->GetDevice()),effect(effect)
{
	Device->AddRef();
}

Shader * Shader::Create(LPD3DXEFFECT effect)
{
	Shader* Instance = new Shader(effect);

	if (FAILED(Instance->Ready_Shader()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT Shader::Ready_Shader()
{

	return S_OK;
}

HRESULT Shader::Begin(UINT passIndex)
{
	if (!effect) return E_FAIL;

	effect->Begin(nullptr, 0);
	effect->BeginPass(passIndex);

	return S_OK;
}

HRESULT Shader::End()
{
	if (!effect) return E_FAIL;

	effect->EndPass();
	effect->End();

	return S_OK;
}


void Shader::SetConstant(const std::string& name, int value)
{
	if (effect)
		effect->SetInt(name.c_str(), value);
}

void Shader::SetConstant(const std::string& name, float value)
{
	if (effect)
		effect->SetFloat(name.c_str(), value);
}

void Shader::SetConstant(const std::string& name, const _vec3& value)
{
	if (effect)
		effect->SetVector(name.c_str(), reinterpret_cast<const D3DXVECTOR4*>(&value));
}

void Shader::SetConstant(const std::string& name, const _matrix& value)
{
	if(effect)
		effect->SetMatrix(name.c_str(), &value);
}

void Shader::SetTexture(const std::string& name, LPDIRECT3DBASETEXTURE9 tex)
{
	if (effect)
		effect->SetTexture(name.c_str(), tex);
}

void Shader::Commit()
{
	if (effect)
		effect->CommitChanges();
}

Shader* Shader::Clone()
{
	Shader* Instance = nullptr;

	if (effect)
	{
		LPD3DXEFFECT cloneEffect = nullptr;
		effect->CloneEffect(Device, &cloneEffect);

		Instance = Shader::Create(cloneEffect);
	}

	return Instance;
}

void Shader::Free()
{
	Safe_Release(effect);
	Safe_Release(Device);
}
