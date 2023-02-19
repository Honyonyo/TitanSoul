#include "stdafx.h"
#include "ObjectManager.h"

void ObjectManager::Init()
{
	_player = new Player;
	m_vObject.push_back(_player);
}

void ObjectManager::Update()
{
	for (auto iter = m_vObject.begin(); iter != m_vObject.end();)
	{
		if (*iter == nullptr)
		{
			iter = m_vObject.erase(iter);
			continue;
		}
		if ((*iter)->GetIsDelete())
		{
			(*iter)->Release();
			iter = m_vObject.erase(iter);
			continue;
		}

		(*iter)->Update();
		iter++;
	}
	for (auto iter = m_vtmpObject.begin(); iter != m_vtmpObject.end();)
	{
		if (*iter == nullptr)
		{
			iter = m_vtmpObject.erase(iter);
			continue;
		}
		if ((*iter)->GetIsDelete())
		{
			(*iter)->Release();
			iter = m_vtmpObject.erase(iter);
			continue;
		}

		(*iter)->Update();
		m_vObject.push_back(*iter);
		iter++;
	}
	m_vtmpObject.clear();
	vector<Object*>q;
	q.swap(m_vtmpObject);


	//##추가 필요 : rectCollision 불값 확인하고 충돌처리 다르게 주기
	for (auto attack = m_vObject.begin(); attack != m_vObject.end(); attack++)
	{
		for (auto hit = m_vObject.begin(); hit != m_vObject.end(); hit++)
		{
			if (attack == hit) 
				continue;
			if (!(*attack)->GetIsOnAttack())
				continue;
			if (!(*hit)->GetIsOnHit())
				continue;

			//공격점과 피격점의 거리가 공격범위+피격범위일 경우 공격자의 Attack함수와 피격자의 Hit함수를 실행시킨다.
			else if (
				MY_UTIL::getDistance((*attack)->GetAttackCenter().x, (*attack)->GetAttackCenter().y, (*hit)->GetHitboxCenter().x, (*hit)->GetHitboxCenter().y)
				<
				(*attack)->GetAttackRange() + (*hit)->GetHitboxRange()
				)
			{
				(*attack)->Attack((*hit)->GetKinds());
				(*hit)->Hit((*attack)->GetKinds());
			}
		}
	}
}

void ObjectManager::Render()
{
	for (auto iter = m_vObject.begin(); iter != m_vObject.end(); iter++)
	{
		if (*iter == nullptr)
		{
			iter = m_vObject.erase(iter);
			continue;
		}
		(*iter)->Render();
	}
}

void ObjectManager::Release()
{
	for (auto iter = m_vObject.begin(); iter != m_vObject.end(); iter++)
	{
		if (*iter == nullptr)
		{
			continue;
		}

		(*iter)->Release();
	}
	m_vObject.clear();
}