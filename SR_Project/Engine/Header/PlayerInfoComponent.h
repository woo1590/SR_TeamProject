#pragma once

#include "ObjectComponent.h"
#include "Subject.h"

BEGIN(Engine)

class ENGINE_DLL PlayerInfoComponent : public ObjectComponent, public Subject
{
public:
	explicit PlayerInfoComponent(Object* owner) : ObjectComponent(owner) {}

	static PlayerInfoComponent* Create(Object* owner);
	HRESULT Ready_Component(Object* owner);

	void SetHp(int hp);
	void AddHp(int amount);
	void AddExp(int amount);

	const PlayerInfo& GetInfo() const { return info; }

private:
	PlayerInfo info{};
};

END