#include "stdafx.h"
#include "GameData.h"

void GameData::Init()
{
	LoadData();
};

void GameData::LoadData()
{
	//JSONDATAMANAGER->loadJsonFile()
}

GameData::GameData()
	:m_stage(0)
{
	m_bossAlived.set();
	for (int i = 0; i < eBossListNumCount; i++)
	{
		m_bossDeadPos[i] = { 0.0f,0.0f };
		m_bossDeadDirection[i] = eDown;
	}
};
