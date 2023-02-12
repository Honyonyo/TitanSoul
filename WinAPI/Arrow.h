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

	float m_speed;	//스피드가 일정 이상일 때만 공격on
	bool m_isShotted;	//쏘아졌는가
	bool m_isDrawed;	//시위 당기고있는가
	bool m_isRetrieved;	//다시 부르고있는가
	bool m_isReady;
	bool m_isOnPickupAni;

	float m_rot;
	float m_rotRadian;

	float m_drawTime;

	PixelCollision* m_pixelCollision;
	//플레이어의 애니메이션을 받아옴(여기서 릴리즈해버리면 뒤져!)
	Animation* m_playerAnimation;
	//플레이어의 중심좌표를 받아옴(delete하면 망해 nullptr로만 바꿔!)
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
		m_attackCenter = { m_center.x + cosf(m_rotRadian) * 4.f , m_center.y + sin(m_rotRadian) * 4.f };//중심점에서 바라보는 각도를 향해 4px만큼 떨어져있음

	}

	//활시위 당기기->화살 쏘기->화살 날아가기
	//화살 다가오기

	//활시위 당기는 중
	void DrawBow(eMoveDirection direction);
	//활 놓음
	void ShotArrow();
	//화살 불러오기
	void Retrieve();
	//화살 회수
	void PickArrow();	//##함수 완성하기

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
			cout << "화살 향하는 각도 계산 불가" << endl;
			cout << "(Arrow::RotateFromDirection()함수 결과값 도출 실패, return 0" << endl;
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

