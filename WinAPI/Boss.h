#pragma once
#include "Object.h"
class BossShield;
class Boss : public Object
{
protected:
	BossShield* m_bossShield;
	bool m_wakeup;

public:
	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

	virtual void Attack(eObjectKinds kinds) PURE;
	virtual void Hit(eObjectKinds kinds) PURE;

	void SetWakeUp() { m_wakeup = true; }
};