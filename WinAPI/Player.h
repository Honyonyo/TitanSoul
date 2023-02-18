#pragma once
#include "Object.h"
#include "Arrow.h"

class Animation;
class PixelCollision;

class Player : public Object
{
	/*
		eObjectKinds m_kinds;
		D2D1_POINT_2F m_center;	//������Ʈ �߽���
		float m_hitboxRange;	//�ǰ� ����
		float m_attackRange;	//���ݹ���

		bool m_isOnHit;
		bool m_isOnAttack;
	*/
	enum eState
	{
		eIdle = 0,
		eWalk,
		eDash,
		eRolling,
		eRollingSuccess,
		eRollingFail,
		eShotting,
		eCall,
		eStageClear,

		eStateNumber
	};

private:
	Animation* m_animation;
	//[����1/����0][����][����]
	vector<int> m_aniIndexArr[2][eMoveDirectionNumCount][eStateNumber];

	PixelCollision* m_pixelCollision;

	Arrow* m_arrow;
	CImage* m_Image;
	CImage* m_bowImage;

	float m_moveSpeed;
	float m_dashSpeed;

	//	LTRB
	bitset<4> m_directionKey;

	int m_soundeffectCount;

	/*
	Ÿ����
	���Ÿ���ϰ�� ���ο����ӿ� switch
	��Ÿ���ϰ�� aniIdxArr[��][][]
	m_isGround = false;(0)
	�ȱ�/����� ��� �Ʊ� ������!=������������ ��� soundOn
	*/
	bool m_inGround;
	eState m_state;

	bool m_aniChange;
	bool m_nonCansleAction;

	bool m_sleep;

private:
	void Move();
	void Rolling();
	void DirectionSetting();

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);

	float GetMoveSpeed(void) { return m_moveSpeed; }
	float GetDashSpeed(void) { return m_dashSpeed; }

	void SetAnimationFrame();
	void SetSleepOnOff(bool sleep) { m_sleep = sleep; }
	void PlaySoundEffect(int prevAniIdx, int nowAniIdx);
	Player();
	~Player();
};