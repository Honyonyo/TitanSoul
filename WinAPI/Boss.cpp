#include "stdafx.h"
#include "Boss.h"

void Boss::SetCollOnOff(bool onoff)
{
	for (int i = 0; i < m_shieldNum; i++)
	{
		m_bossShield[i]->SetHitboxOnOff(onoff);
	}
}