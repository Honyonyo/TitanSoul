#pragma once
#include "Boss.h"
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
	float m_shoulderPosL;	//���ݽ� ��/ ��� 3�������� �����̱�. �ݴ�� ������ --, �ݴ�� ������ Ÿ�ֿ̹� 0���� �����
	float m_shoulderPosR;
	float m_floatingPos;	//���Ʒ��� �սǵս�. center y�� ���ؼ� render, �ִ� 3���� �þ�ٰ� �ٽ� �������⸦ �ݺ��ϱ�
	bool m_floatingUp;

	bool m_ready;
	int m_headImageIdx;

	float m_sceneChangeAlpha;


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

	//���ø���� ��Ⱑ ���� 1�ʾ� ��
	D2D1_POINT_2F* m_colossusCenter;

	float m_moveSpeed;
	float m_downSpeed;

	bool m_leftHand;
	bool m_isGround;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);
	
	void HandUp();
	void HandDown();
	void ComeBack();

	ColossusHand(D2D1_POINT_2F* colossusCenter, bool leftHand);
	~ColossusHand();
};