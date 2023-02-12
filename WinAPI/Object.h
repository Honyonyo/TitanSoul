#pragma once
#include "enumList.h"
class Object
{
protected:
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

public:
	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;

	virtual void Attack(eObjectKinds kinds) PURE;
	virtual void Hit(eObjectKinds kinds) PURE;	//�� �������� ������ ���� �ٸ� ���� �����ֱ�

#pragma region ������ ������
	eObjectKinds GetKinds() { return m_kinds; }
	
	D2D1_POINT_2F GetPointF() { return m_center; }
	POINT GetPoint() { return { (long)m_center.x, (long)m_center.y }; }
	float GetX() { return m_center.x; }
	float GetY() { return m_center.y; }

	float GetHitboxRange() { return m_hitboxRange; }
	float GetAttackRange() { return m_attackRange; }
	bool GetIsOnHit() { return m_isOnHit; }
	bool GetIsOnAttack() { return m_isOnAttack; }
	D2D1_POINT_2F GetHitboxCenter(){ return m_hitboxCenter; }
	D2D1_POINT_2F GetAttackCenter(){ return m_attackCenter; }
	bool GetIsDelete() { return m_isDelete; }


	void SetKinds(eObjectKinds kinds) { m_kinds = kinds; }
	void SetPosition(float x, float y) { m_center.x = x; m_center.y = y; }
	void SetPosition(D2D1_POINT_2F center) { m_center=center; }
	void SetPosition(POINT center) { m_center.x = center.x; m_center.y = center.y; }
	void SetPosition(long x, long y) { m_center.x = x; m_center.y = y; }

	//����ó�� �ȵǾ������Ƿ� �� �����ڷ����� �ֱ�
	template<class T>
	void SetHitBoxRnage(T range) { m_hitboxRange = (float)range; }

	//����ó�� �ȵǾ������Ƿ� �� �����ڷ����� �ֱ�
	template<class T>
	void SetAttackRange(T range) { m_attackRange = (float)range; }
	void SetHitboxOnOff(bool onoff) { m_isOnHit = onoff; }
	void SetAttackOnOff(bool onoff) { m_isOnAttack = onoff; }
	
	void ObjectDelete() { m_isDelete = true; }

#pragma endregion

	Object() 
		: m_center(D2D1::Point2F(0,0)), m_hitboxRange(0), m_attackRange(0),
		m_isOnHit(false), m_isOnAttack(false), m_isAlive(true), m_isDelete(false)
	{
	}
};

