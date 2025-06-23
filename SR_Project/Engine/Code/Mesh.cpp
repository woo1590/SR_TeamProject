#include "Mesh.h"
#include "GraphicDevice.h"

Mesh::Mesh()
	:Device(GraphicDevice::GetInstance()->GetDevice())
{
	Device->AddRef();
}

Mesh::~Mesh()
{
}

void Mesh::Free()
{
	Safe_Release(Device);
}
