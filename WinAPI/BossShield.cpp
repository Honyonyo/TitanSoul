#include "stdafx.h"
#include "BossShield.h"
#include "Boss.h"

void BossShield::Init()
{
	m_kinds = eMonster;
}

void BossShield::Update()
{
	m_center = m_hitboxCenter = *m_bossCenter;
}

void BossShield::Render()
{
}

void BossShield::Release()
{
}

void BossShield::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eArrow:
		m_boss->SetWakeUp();
		break;

	default:
		break;
	}
}

BossShield::BossShield()
{
	m_isDelete = false;
	m_isOnHit = true;
	m_isOnAttack = false;
}

BossShield::~BossShield()
{
}
