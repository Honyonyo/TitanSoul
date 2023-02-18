#include "stdafx.h"
#include "Sludgeheart.h"

void Sludgeheart::Init()
{
	GAMEMANAGER->GetBossState(eSludgeheart, &m_isAlive, &m_center, &m_direction);

	m_shadow = IMAGEMANAGER->FindImage("SludgeheartShadow");
	m_img = IMAGEMANAGER->FindImage("Sludgeheart");

	//m_center = { 700,900 };
	m_splitCount = 2;
	SetPos();
	SetRange();

	m_isOnAttack = true;
}

void Sludgeheart::Update()
{
	m_scale += 0.05;

	if (KEYMANAGER->isOnceKeyDown(VK_UP))
	{
		m_splitCount++;
		if (m_splitCount > m_splitCountMax)
		{
			m_splitCount = 0;
			//SetPos();
			SetRange();
		}
	}
	if (KEYMANAGER->isOnceKeyDown(VK_DOWN))
	{
		m_splitCount--;
		if (m_splitCount < 0)
		{
			m_splitCount = m_splitCountMax;
			//SetPos();
			SetRange();
		}
	}
}

void Sludgeheart::Render()
{
	float cos5 = cosf(m_scale)*0.5;
	float cos25 = cos5 * 0.5;

	eLayer layer = (m_center.y > PLAYER->GetPointF().y) ? eLayerUpperPlayer : eLayerUnderPlayer;
	IMAGEMANAGER->CenterFrameRender(m_shadow, m_center.x, m_center.y, m_splitCount, 0, eLayerShadow,1,1,0,0.3);
	IMAGEMANAGER->CenterFrameRender(m_shadow, m_center.x+32, m_center.y + m_attackRange / 2, m_splitCount, 0, eLayerShadow, { 64, 32*1 }, 1 + cos25, 1 - cos25, 0, 0.3);
	IMAGEMANAGER->CenterFrameRender(m_img, m_hitboxCenter.x + 32, m_hitboxCenter.y+60, m_splitCount, 0, layer, { 64, 128}, 1 + cos25, 1 - cos25, 0, 0.3);
}

void Sludgeheart::Release()
{
	m_isDelete = true;
}

void Sludgeheart::Attack(eObjectKinds kinds)
{
	switch (kinds)
	{
	case ePlayer:
		cout << "물컹이 플레이어 공격" << endl;
		break;
	}
}

void Sludgeheart::Hit(eObjectKinds kinds)
{
	// 분열 단계가 4인상태에서 맞으면 하트 보유자가 아닌 경우 delete
}

void Sludgeheart::SetRange()
{
	switch (m_splitCount)
	{
	case 0: 
		m_hitboxRange = 64;
		m_attackRange = 32;
		break;
	case 1:
		m_hitboxRange = 38;
		m_attackRange = 20;
		break;
	case 2:
		m_hitboxRange = 25;
		m_attackRange = 13;
		break;
	case 3:
		m_hitboxRange = 15;
		m_attackRange = 10;
		break;
	case 4:
		m_hitboxRange = 7;
		m_attackRange = 4;
		break;
	default:
		cout << "분열카운터가 잘못됨. 객체 지움" << endl;
		m_isDelete = true;
	}
}

void Sludgeheart::SetPos()
{
	m_attackCenter = m_center;
	m_hitboxCenter.x = m_center.x;
	m_hitboxCenter.y = m_center.y - 32;
}

D2D1_POINT_2F Sludgeheart::CalculationSplitPos(D2D1_POINT_2F* origin, D2D1_POINT_2F* split)
{


	return D2D1_POINT_2F();
}

Sludgeheart::Sludgeheart()
	:m_shadow(nullptr), m_img(nullptr), m_scale(0),
	m_isHeart(true), m_splitCount(0), m_splitCountMax(4),
	m_isJumped(false), m_jumpSpeed(0), m_speed(5)
{
	m_isOnHit = true;
	m_isOnAttack = true;
	m_isDelete = false;
	m_isAlive = true;
	m_direction = eUp;

	m_center = { 497, 356 };
	m_attackCenter = m_center;
	m_hitboxCenter.x = m_attackCenter.x;
	m_hitboxCenter.y = m_attackCenter.y - 32;
	m_hitboxRange = 64;
	m_attackRange = 32;

	m_isHeart = true;
}

Sludgeheart::Sludgeheart(int splitCount, D2D1_POINT_2F splitPos)
	:m_shadow(nullptr), m_img(nullptr), m_scale(0),
	m_isHeart(false), m_splitCount(splitCount),m_splitCountMax(4),
	m_isJumped(false), m_jumpSpeed(0), m_speed(5)
{
	m_isOnHit = true;
	m_isOnAttack = true;
	m_isDelete = false;
	m_isAlive = true;
	m_direction = eUp;

	m_center = splitPos;
	SetRange();
	m_isHeart = true;
}

Sludgeheart::~Sludgeheart()

{
}
