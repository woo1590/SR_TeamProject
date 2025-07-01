#pragma once

#include "ProgressBar.h"

BEGIN(Engine)

class ENGINE_DLL ExpComponent : public ProgressBar
{
private:
	explicit ExpComponent(Object* owner)
		:ProgressBar(owner) {}

public:
	static ExpComponent* Create(Object* owner);

protected:
	void ExtractValues(const void* data, int& cur, int& max) override;
};

END