#pragma once
#include "Object.h"

class Animation;
class PixelCollision;
class Arrow : public Object
{
private:
	CImage* m_image;
	CImage* m_bigImage;
	Animation* m_animation;
	int m_pickupAniArr[4];
	D2D1_POINT_2F m_pickupPoint;

	float m_speed;	//���ǵ尡 ���� �̻��� ���� ����on
	bool m_isShotted;	//������°�
	bool m_isDrawed;	//���� �����ִ°�
	bool m_isRetrieved;	//�ٽ� �θ����ִ°�
	bool m_isReady;
	bool m_isOnPickupAni;

	float m_rot;
	float m_rotRadian;

	float m_drawTime;

	PixelCollision* m_pixelCollision;
	//�÷��̾��� �ִϸ��̼��� �޾ƿ�(���⼭ �������ع����� ����!)
	Animation* m_playerAnimation;
	//�÷��̾��� �߽���ǥ�� �޾ƿ�(delete�ϸ� ���� nullptr�θ� �ٲ�!)
	D2D1_POINT_2F* m_playerCenter;


public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	void Move();

	virtual void Attack(eObjectKinds kinds)
	{
		switch (kinds)
		{
		case ePlayer:
			//PickArrow();
			break;
		case eMonster:
			m_speed = 0;
		}
	}
	virtual void Hit(eObjectKinds kinds)
	{
		switch (kinds)
		{
		case eMonster:
			break;

		default:
			NULL;
		}
	}

	bool GetIsReady() { return m_isReady; }
	bool GetIsShooted() { return m_isShotted; }
	bool GetIsDrawed() { return m_isDrawed; }
	bool GetIsRetrieved() { return m_isRetrieved; }
	void SetIsReady(bool ready) { m_isReady = ready; }
	void SetSpeed(float speed) { m_speed = speed; }
	void SetRot(float rotate) { m_rot = rotate; }
	void SetAttackRange()
	{
		m_attackCenter = { m_center.x + cosf(m_rotRadian) * 4.f , m_center.y + sin(m_rotRadian) * 4.f };//�߽������� �ٶ󺸴� ������ ���� 4px��ŭ ����������

	}

	//Ȱ���� ����->ȭ�� ���->ȭ�� ���ư���
	//ȭ�� �ٰ�����

	//Ȱ���� ���� ��
	void DrawBow(eMoveDirection direction);
	//Ȱ ����
	void ShotArrow();
	//ȭ�� �ҷ�����
	void Retrieve();
	//ȭ�� ȸ��
	void PickArrow();	//##�Լ� �ϼ��ϱ�

	float RotateFromDirection(eMoveDirection direction)
	{
		switch (direction)
		{
		case eLeft:return 180;
		case eLeftUp:return 225;
		case eUp:return 270;
		case eRightUp:return 315;
		case eRight:return 0;
		case eRightDown:return 45;
		case eDown:return 90;
		case eLeftDown:return 135;
		default:
			cout << "ȭ�� ���ϴ� ���� ��� �Ұ�" << endl;
			cout << "(Arrow::RotateFromDirection()�Լ� ����� ���� ����, return 0" << endl;
			cout << endl;
			return NULL;
		}
	}

	Arrow() : m_speed(0), m_isShotted(false), m_playerAnimation(nullptr)
	{
	}
	Arrow(Animation* ani, D2D1_POINT_2F* center) :
		 m_isShotted(false), m_isDrawed(false), m_isRetrieved(false),
		m_speed(0), m_rot(0), m_rotRadian(0), m_drawTime(0.0f),
		m_isReady(true), m_isOnPickupAni(false)
	{
		m_playerAnimation = ani;
		m_playerCenter = center;
	}
};

