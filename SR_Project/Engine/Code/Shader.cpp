#include "EnginePCH.h"
#include "Shader.h"
#include "GraphicDevice.h"

Shader::Shader(LPDIRECT3DVERTEXSHADER9* vs, LPDIRECT3DPIXELSHADER9* ps)
	:Device(GraphicDevice::GetInstance()->GetDevice()),VS(vs),PS(ps)
{
	Device->AddRef();
}

Shader* Shader::Create(LPDIRECT3DVERTEXSHADER9* vs, LPDIRECT3DPIXELSHADER9* ps)
{
	Shader* Instance = new Shader(vs,ps);

	if (FAILED(Instance->Ready_Shader()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

void Shader::Apply()
{
	Device->SetVertexShader(VS);
	Device->SetPixelShader(PS);
}

void Shader::SetConstant(const std::wstring& name, int value)
{
}

void Shader::SetConstant(const std::wstring& name, float value)
{
}

void Shader::SetConstant(const std::wstring& name, _vec3 value)
{
}

void Shader::SetConstant(const std::wstring& name, _matrix value)
{
}
