#pragma once

class _declspec(dllexport) Base
{
protected:
	inline explicit Base();
	inline virtual ~Base();

public:
	inline unsigned long AddRef();
	inline unsigned long Release();
	inline virtual void Free() = 0;

private:
	unsigned long m_dwRefCnt;

};

#include "Base.inl"