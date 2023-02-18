#pragma once
#include "Boss.h"
class Colossus : public Boss
{
private:

	/*
	eObjectKinds m_kinds;
	D2D1_POINT_2F m_center;	//오브젝트 중심점
	float m_hitboxRange;	//피격 범위
	D2D_POINT_2F m_hitboxCenter;	//피격범위의 중심점
	float m_attackRange;	//공격범위
	D2D1_POINT_2F m_attackCenter;		//공격범위의 중심점

	bool m_isOnHit;
	bool m_isOnAttack;
	bool m_isDelete;

	bool m_isAlive;
	*/

	enum eParts
	{
		eBody = 0,
		eBodyLight,
		eHand,
		eHandShadow,
		eHandSleep,
		eHead,

		ePartsNumCount
	};
	CImage* m_image[ePartsNumCount];

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);

	Colossus();
	~Colossus();
};

