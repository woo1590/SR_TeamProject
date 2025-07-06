#include "EnginePCH.h"
#include "Shader.h"
#include "GraphicDevice.h"

Shader::Shader()
	:Device(GraphicDevice::GetInstance()->GetDevice())
{
	Device->AddRef();
}

Shader * Shader::Create(const void * vsCode, const void* psCode)
{
	Shader* Instance = new Shader;

	if (FAILED(Instance->Ready_Shader(vsCode, psCode)))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT Shader::Ready_Shader(const void* vsCode, const void* psCode)
{
	if (vsCode)
	{
		Device->CreateVertexShader((DWORD*)vsCode, &VS);
		D3DXGetShaderConstantTable((DWORD*)vsCode, &vsConst);
	}
	Device->CreatePixelShader((DWORD*)psCode, &PS);

	D3DXGetShaderConstantTable((DWORD*)psCode, &psConst);

	return S_OK;
}

void Shader::Apply()
{
	Device->SetVertexShader(VS);
	Device->SetPixelShader(PS);
}

void Shader::SetConstant(const std::string& name, int value)
{
	D3DXHANDLE h = vsConst->GetConstantByName(nullptr, name.c_str());
	if (h)
		vsConst->SetInt(Device, h, value);
	
	h = psConst->GetConstantByName(nullptr, name.c_str());
}

void Shader::SetConstant(const std::string& name, float value)
{
}

void Shader::SetConstant(const std::string& name, _vec3 value)
{
}

void Shader::SetConstant(const std::string& name, _matrix value)
{
}
