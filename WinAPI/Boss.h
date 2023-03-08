#pragma once
#include "Object.h"
#include "BossShield.h"

class Boss : public Object
{
protected:
	BossShield** m_bossShield;
	int m_shieldNum;
	bool m_wakeup;

public:
	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

	virtual void Attack(eObjectKinds kinds) PURE;
	virtual void Hit(eObjectKinds kinds) PURE;

	void SetCollOnOff(bool onoff);

	void SetWakeUp() { m_wakeup = true; }
};