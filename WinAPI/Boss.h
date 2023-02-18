#pragma once
#include "Object.h"
class Boss : public Object
{
protected:
	bool m_wakeup;

private:
	float m_deffendCircle;
	RECT m_deffendRect;

public:
	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

	virtual void Attack(eObjectKinds kinds) PURE;
	virtual void Hit(eObjectKinds kinds) PURE;
};