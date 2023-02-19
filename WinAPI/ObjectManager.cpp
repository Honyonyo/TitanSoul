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


	//##�߰� �ʿ� : rectCollision �Ұ� Ȯ���ϰ� �浹ó�� �ٸ��� �ֱ�
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

			//�������� �ǰ����� �Ÿ��� ���ݹ���+�ǰݹ����� ��� �������� Attack�Լ��� �ǰ����� Hit�Լ��� �����Ų��.
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