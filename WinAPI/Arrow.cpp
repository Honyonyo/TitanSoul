#include "stdafx.h"
#include "Arrow.h"
#include "Animation.h"
#include "PixelCollision.h"
void Arrow::Init()
{
	m_image = IMAGEMANAGER->FindImage("ArrowImageSheet");
	m_bigImage = IMAGEMANAGER->FindImage("PlayerImageSheet");
	for (int i = 0; i < 4; i++)
	{
		m_pickupAniArr[i] = 6 * 31 + 28 + i;
	}
	m_animation = new Animation;
	m_animation->Init(m_image);
	m_animation->SetPlayFrame(m_pickupAniArr, 4, false);

	m_kinds = eArrow;
	m_isOnHit = false;
	m_isOnAttack = true;
	m_isAlive = true;
	m_attackRange = 6;

	m_pixelCollision = new PixelCollision(&m_attackCenter, 0, 0, m_attackRange, m_attackRange);
	m_pixelCollision->Init();
};
void Arrow::Update()
{
	if (m_isShotted)
	{
		Move();
		m_pixelCollision->Update();
	}
	if (m_speed > 0.3)
	{
		m_isOnAttack = true;
	}
	else m_isOnAttack = false;

	SetAttackRange();

	if (m_isShotted)
	{
		if (m_direction == eUp)
		{
			if (MY_UTIL::getDistance(m_attackCenter.x, m_attackCenter.y - 4, m_playerCenter->x, m_playerCenter->y) < 3)
			{
				cout << MY_UTIL::getDistance(m_attackCenter.x, m_attackCenter.y, m_playerCenter->x, m_playerCenter->y) << endl;
				PickArrow();
			}
		}
		else if (MY_UTIL::getDistance(m_attackCenter.x, m_attackCenter.y, m_playerCenter->x, m_playerCenter->y) < 10)
		{
			PickArrow();
		}
	}

	if (m_isOnPickupAni)
	{
		if (!m_animation->IsPlay())
		{
			m_isOnPickupAni = false;
		}
	}
};
void Arrow::Render()
{
	eLayer layer;
	if (m_direction == eLeftUp || m_direction == eUp || m_direction == eRightUp)
		layer = eLayerUnderPlayer;
	else layer = eLayerPlayer;
	//Ȱ���� ��涧
	if (m_isDrawed)
	{
		IMAGEMANAGER->CenterFrameRender(m_bigImage, m_center.x, m_center.y, 31, 0, layer, 1, 1, m_rot);
	}
	//Ȱ���� �ȴ�� �� (�б� ��Ҵ°� �ƴѰ��� ����)
	else
	{
		if (m_isShotted)
		{
			IMAGEMANAGER->CenterFrameRender(m_bigImage, m_center.x, m_center.y, 31, 0, layer, 1, 1, m_rot);
		}
		else
		{
			IMAGEMANAGER->CenterAniRender(m_image, m_playerCenter->x, m_playerCenter->y, m_playerAnimation, eLayerPlayer);
		}
	}

	if (m_isOnPickupAni)
	{
		IMAGEMANAGER->CenterAniRender(m_image, m_pickupPoint.x, m_pickupPoint.y, m_animation, layer);
	}
};
void Arrow::Release()
{
	m_playerCenter = nullptr;
	m_playerAnimation = nullptr;

	SAFE_DELETE(m_animation);
};

void Arrow::DrawBowStart(eMoveDirection direction)
{
	m_isDrawed = true;
	m_center = *m_playerCenter;
	m_center.y += 4;
	m_direction = direction;
	m_rot = RotateFromDirection(direction);
	m_rotRadian = m_rot * DEG_TO_RAD;
}

eMoveDirection Arrow::DrawBow()
{
	if (RotateChangeOnDrawBow())
	{
		m_direction = DirectionFromRotate();
	}
	m_rotRadian = m_rot * DEG_TO_RAD;
	m_drawTime = (m_drawTime > 1) ? 1 : m_drawTime + DELTA_TIME;
	return m_direction;
};

void Arrow::ShotArrow()
{
	m_isDrawed = false;
	m_isShotted = true;
	m_isReady = false;

	if (m_drawTime < 0.1f)
	{
		m_isShotted = false;
		m_speed = 0;
	}
	else
	{
		m_speed = round(m_drawTime * 200) * 0.1f;
		if (m_speed > 20) m_speed = 20;
		m_center.x += 6 * cosf(m_rotRadian);
		m_center.y -= 6 * sinf(m_rotRadian);
		SetAttackRange();
	}

	m_drawTime = 0.0f;
};

void Arrow::Move()
{
	if (m_speed > 0)
	{
		m_center.x += m_speed * cosf(m_rotRadian);
		m_center.y += m_speed * sinf(m_rotRadian);
		m_speed = (m_speed < 0) ? 0 : m_speed - DELTA_TIME * 10;
	}
	else
	{
		m_isRetrieved = false;
	}
	if (m_pixelCollision->GetIsColl())
	{
		m_speed = 0;
	}
};

void Arrow::Retrieve()
{
	if (!m_isRetrieved)
	{
		m_rotRadian = -MY_UTIL::getAngle(m_center.x, m_center.y, m_playerCenter->x, m_playerCenter->y);
		m_rot = m_rotRadian * RAD_TO_DEG;
	}
	m_isRetrieved = true;
	m_speed = (m_speed > 10) ? 10 : m_speed + DELTA_TIME * 12;
};

void Arrow::PickArrow()
{
	cout << "ȭ�� �ֿ���" << endl;
	m_speed = 0;
	m_isShotted = false;
	m_isDrawed = false;
	m_isRetrieved = false;
	m_drawTime = 0;

	m_isOnPickupAni = true;
	m_pickupPoint = m_attackCenter;
	m_animation->AniStart();
};

bool Arrow::RotateChangeOnDrawBow()
{
	bool changed = false;
	float changeRot = 3 * DELTA_TIME;
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (m_rot < 180.f)
		{
			m_rot += 45.f * changeRot;
			if (m_rot >= 180.f) m_rot = 180.f;
		}
		else
		{
			m_rot -= 45 * changeRot;
			if (m_rot < 0) m_rot = 0.f;
		}
		changed = true;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (m_rot < 180.f)
		{
			m_rot -= 45.f * changeRot;
			if (m_rot < 0) m_rot = 0.f;
		}
		else
		{
			m_rot += 45.f * changeRot;
			if (m_rot >= 360.f) m_rot = 0.f;
		}
		changed = true;
	}
	//����Ű ������ �� (������ 90~270�϶� ��Ű = �� ���ϱ� / �ƴҶ� �� ����)
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (m_rot > 90.f && m_rot <= 270.f)
		{
			m_rot += 45.f * changeRot;
			if (m_rot > 270.f) m_rot = 270.f;
		}
		else
		{
			m_rot -= 45.f * changeRot;
			if (m_rot < 0.f) m_rot += 360;
			else if (m_rot < 270.f) m_rot = 270.f;
		}
		changed = true;
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (m_rot >= 90.f && m_rot <= 270.f)
		{
			m_rot -= 45.f * changeRot;
			if (m_rot < 90.f) m_rot = 90.f;
		}
		else
		{
			m_rot += 45.f * changeRot;
			if (m_rot >= 360) m_rot = 0.f;
			else if (m_rot > 90.f) m_rot = 90.f;
		}
		changed = true;
	}

	return 	changed;
};
