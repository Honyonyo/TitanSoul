#pragma once
#include "Boss.h"
class ColossusHand;
class Colossus : public Boss
{
private:
	enum eParts
	{
		eBody = 0,
		eBodyLight,
		eShoulder,
		eHead,

		ePartsNumCount
	};
	CImage* m_image[ePartsNumCount];
	float m_shoulderPosL;	//공격시 좌 / 우로 3정도까지 움직이기. 반대손 켜지면 --, 반대손 공격할 타이밍에 0으로 만들기
	float m_shoulderPosR;
	float m_floatingPos;	//위아래로 둥실둥실. center y에 더해서 render, 최대 3까지 늘어났다가 다시 내려오기를 반복하기
	bool m_floatingUp;

	bool m_ready;
	int m_headImageIdx;

	float m_sceneChangeAlpha;
	bool m_hitpointEffectOn;
	float m_hitpointEffectAlpha;

	ColossusHand* m_handL;
	ColossusHand* m_handR;
	bool m_moveLeftHand;
	void Attack();

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);

	void SetFloor();
	void SetActionScene();
	Colossus();
	~Colossus();
};

class ColossusHand : public Object
{
	CImage* m_image;
	CImage* m_shadow;
	int m_imageFrame;

	//들어올리기와 찍기가 각각 1초씩 들어감
	D2D1_POINT_2F m_colossusCenter;
	D2D1_POINT_2F m_moveStartP;

	float m_moveSpeed;
	float m_actionTime;

	bool m_leftHand;
	bool m_attack;
	bool m_up;
	bool m_down;
	bool m_comeback;

	bool m_playHitEffectSound;
public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);
	
	void AttackStart()
	{
		m_moveStartP = m_center;

		m_attack = true;
		m_up = true;
		m_down = false;
		m_comeback = false;
		m_imageFrame = 0;
		m_actionTime = 0.f;

		m_isOnAttack = false;
		m_isOnHit = false;
		m_playHitEffectSound = false;
		SOUNDMANAGER->play("RiseBodyLand", 1.f);
	};

	void HandUp();
	void HandDown();
	void ComeBack();

	inline bool GetIsAttacking() { return m_attack; }
	inline bool GetIsOnDeffend() 
	{
		if (m_hitboxCenter.x == m_colossusCenter.x && m_hitboxCenter.y == m_colossusCenter.y)
			return true;
		else return false;
	}
	void SetImageFrame(int frameNumber)
	{
		m_imageFrame = frameNumber;
	};
	void PlaySoundEffect();	
	ColossusHand(bool leftHand);
	~ColossusHand();
};