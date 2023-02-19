#pragma once
#include "Object.h"
class Boss;
class BossShield : public Object
{
private:
	Boss* m_boss;
	D2D1_POINT_2F* m_bossCenter;
	float m_centerDistanceX, m_centerDistanceY;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds){};
	virtual void Hit(eObjectKinds kinds);

	void SetBoss(Boss* boss) { m_boss = boss; };
	void Setting(Boss* boss, D2D1_POINT_2F* center, float centerDistanceX, float centerDistanceY, float range)
	{
		m_boss = boss;
		m_bossCenter = center;
		m_centerDistanceX = centerDistanceX;
		m_centerDistanceY = centerDistanceY;
		m_hitboxRange = range;
		m_hitboxCenter = *center;
	};

	BossShield();
	~BossShield();
};

