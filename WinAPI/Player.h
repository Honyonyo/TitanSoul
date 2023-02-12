#pragma once
#include "Object.h"
#include "Arrow.h"

class Animation;
class PixelCollision;

class Player : public Object
{
	/*
		eObjectKinds m_kinds;
		D2D1_POINT_2F m_center;	//오브젝트 중심점
		float m_hitboxRange;	//피격 범위
		float m_attackRange;	//공격범위

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
	//[지상1/물속0][방향][동작]
	vector<int> m_aniIndexArr[2][eMoveDirectionNumber][eStateNumber];

	PixelCollision* m_pixelCollision;

	Arrow* m_arrow;
	CImage* m_Image;
	CImage* m_bowImage;

	float m_moveSpeed;
	float m_dashSpeed;

	bitset<4> m_directionKey;

	bool m_inGround;
	eState m_state;

	bool m_aniChange;

private:
	void Move();

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
	Player();
	~Player();
};

