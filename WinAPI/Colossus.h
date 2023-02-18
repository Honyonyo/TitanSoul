#pragma once
#include "Boss.h"
class Colossus : public Boss
{
private:

	/*
	eObjectKinds m_kinds;
	D2D1_POINT_2F m_center;	//������Ʈ �߽���
	float m_hitboxRange;	//�ǰ� ����
	D2D_POINT_2F m_hitboxCenter;	//�ǰݹ����� �߽���
	float m_attackRange;	//���ݹ���
	D2D1_POINT_2F m_attackCenter;		//���ݹ����� �߽���

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

