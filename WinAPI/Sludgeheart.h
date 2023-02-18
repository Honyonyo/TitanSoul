#pragma once
#include "Object.h"
class Sludgeheart : public Object
{
private:
	/*
		eObjectKinds m_kinds;
	D2D1_POINT_2F m_center;	//������Ʈ �߽���
	float m_hitboxRange;	//�ǰ� ����
	D2D_POINT_2F m_hitboxCenter;	//�ǰݹ����� �߽���
	float m_attackRange;	//���ݹ���
	D2D1_POINT_2F m_attackCenter;		//���ݹ����� �߽���

	eMoveDirection m_direction;

	bool m_isOnHit;
	bool m_isOnAttack;
	bool m_isDelete;

	bool m_isAlive;
*/
	CImage* m_shadow;
	CImage* m_img;
	float m_scale;

	bool m_isHeart;
	const int m_splitCountMax;
	int m_splitCount;

	bool m_isJumped;
	float m_jumpSpeed;
	float m_speed;

public:
	//�׸��ڼ��͸� attackCenter�� ���߱�
	//img���͸� hitCenter�� ���߱�
	//�����ϸ� hit, attack������
	//

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);	//�� �������� ������ ���� �ٸ� ���� �����ֱ�

	void SetRange();
	void SetPos();
	
	//�п� �� ���� ����Ʈ ����
	D2D1_POINT_2F CalculationSplitPos(D2D1_POINT_2F* origin, D2D1_POINT_2F* split);


	Sludgeheart();
	//�п�ü ������ (��ܰ�п�ü�ΰ�, ���� ��ǥ)
	//ó������ �ǰݽ� center��ǥ �߽����� Ư�� �Ÿ���ŭ ���� �ݴ�� ������ �� ��ҿ� ���� ������� ���� �����ȳ��� ���� ���´�. rnd�� ��
	//��ǥ ����ؼ� �����ڿ� �ְ� OBJMANAGER�� �ֱ�
	Sludgeheart(int splitCount,D2D1_POINT_2F splitPos);
	~Sludgeheart();
};

