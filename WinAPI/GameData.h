#pragma once
#include "SingletonBase.h"
#include "enumList.h"

const static enum eBossList
{
	eYeti = 0,
	eSludgeheart,
	eColossus,

	eBossListNumCount
};

class GameData : public SingletonBase<GameData>
{
private:
	//0 : Á×À½ 1 »ýÁ¸
	bitset<eBossListNumCount> m_bossAlived;
	D2D1_POINT_2F m_bossDeadPos[eBossListNumCount];
	eMoveDirection m_bossDeadDirection[eBossListNumCount];

	int m_stage;

public:
	void Init();

	void LoadData();

	bool GetBossAlive(eBossList boss) { return m_bossAlived[boss]; }
	D2D1_POINT_2F GetBossDeadPosition(eBossList boss) { return m_bossDeadPos[boss]; }
	eMoveDirection GetBossDeadDirection(eBossList boss) { return m_bossDeadDirection[boss]; }
	void GetBossState(eBossList boss, bool* isAlived, D2D1_POINT_2F* point, eMoveDirection* direction) { 
		*isAlived = m_bossAlived[boss];
		if (!m_bossAlived[boss])
		{
			*point = m_bossDeadPos[boss];
			*direction = m_bossDeadDirection[boss];
		}
		else
		{
			*direction = eDown;
		}
	}
	void SetBossAlive(eBossList boss) { m_bossAlived[boss] = 1; }
	void SetBossDead(eBossList boss, D2D1_POINT_2F pos, eMoveDirection direction)
	{
		m_bossAlived[boss] = 0;
		m_bossDeadPos[boss] = pos;
		m_bossDeadDirection[boss] = direction;
	}
	void SetBossDead(eBossList boss) { m_bossAlived[boss] = 0; }
	void SetBossDeadPosition(eBossList boss, float x, float y) { m_bossDeadPos[boss] = { x, y }; }
	void SetBossDeadPosition(eBossList boss, D2D1_POINT_2F pos) { m_bossDeadPos[boss] = pos; }
	void SetBossDeadDirection(eBossList boss, eMoveDirection direction) { m_bossDeadDirection[boss] = direction; }

	GameData();
	~GameData() {}
};