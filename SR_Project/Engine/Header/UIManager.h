#pragma once

BEGIN(Engine)

class ENGINE_DLL UIManager : public Base
{
private:
	explicit UIManager() {}
	virtual ~UIManager() {}

public:
	static UIManager* Create();
	
	HRESULT Ready_UIManager();

private:
	void Free() override {}
};

END