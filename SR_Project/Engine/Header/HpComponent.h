#pragma once

#include "ProgressBar.h"

BEGIN(Engine)

class ENGINE_DLL HpComponent : public ProgressBar
{
private:
	explicit HpComponent(Object* owner)
		:ProgressBar(owner) {
	}

public:
	static HpComponent* Create(Object* owner);

protected:
	void ExtractValues(const void* data, int& cur, int& max) override;
};

END