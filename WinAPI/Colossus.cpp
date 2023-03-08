#include "stdafx.h"
#include "Colossus.h"
#include "Camera.h"

void Colossus::Attack()
{
}

void Colossus::Init()
{
	//Image Setting
	m_image[eBody] = IMAGEMANAGER->FindImage("ColossusBody");
	m_image[eBodyLight] = IMAGEMANAGER->FindImage("ColossusBodyLight");
	m_image[eShoulder] = IMAGEMANAGER->FindImage("ColossusHandSleep");
	m_image[eHead] = IMAGEMANAGER->FindImage("ColossusHead");

	GAMEMANAGER->GetBossState(eColossus, &m_isAlive, &m_center, &m_direction);

	m_hitboxCenter = m_center;
	m_hitboxRange = 4.f;

	OBJECTMANAGER->AddObject(m_handL);
	OBJECTMANAGER->AddObject(m_handR);
}

void Colossus::Update()
{
	if (m_isAlive)
	{
		//if alive -> if sleep -> collision -> camera shake & sceneChange Colossus
		//if alive -> if !sleep -> collision&die -> camera shake & sceneChange Floor


		//if !wake 머리가 바닥에
		//if ready 눈번쩍, 화면 흔들흔들(피격판정 off)
		//씬전환 후 alive, wake, !ready
		if (m_wakeup)
		{
			if (!m_handL->GetIsAttacking() && !m_handR->GetIsAttacking())
			{
				//플레이어가 오른쪽에 있을 때
				if (PLAYER->GetPointF().x > m_center.x)
				{
					m_handR->AttackStart();
				}
				else
				{
					m_handL->AttackStart();
				}
			}
		}
		else
		{
			if (m_ready)
			{
				if (m_sceneChangeAlpha > 1)
				{
					m_ready = false;
					m_wakeup = true;
					m_sceneChangeAlpha = NULL;
					m_headImageIdx = 0;
					CAMERA->SetCameraShakingOff();
					CAMERA->SetCameraMove(PLAYER->GetPointF(), true, false, 3);
					PLAYER->ResetPlayer();
					return;
				}
				m_sceneChangeAlpha += DELTA_TIME * 0.4;
			}
			else
			{

			}
		}


	}
}

void Colossus::Render()
{
	if (!m_wakeup)
	{
		IMAGEMANAGER->CenterFrameRender(m_image[eHead], m_center.x, m_center.y + 16, m_headImageIdx, 0, eLayerUnderPlayer);
		if (m_ready)
		{
			IMAGEMANAGER->DrawBlackRect({ 0,0,WINSIZE_X, WINSIZE_Y }, eLayerTop, m_sceneChangeAlpha);
		}
	}
	else
	{
		IMAGEMANAGER->CenterFrameRender(m_image[eBody], m_center.x, m_center.y, 0, 0, eLayerUnderPlayer);
		IMAGEMANAGER->CenterFrameRender(m_image[eHead], m_center.x, m_center.y - 40, m_headImageIdx, 0, eLayerUnderPlayer);
		IMAGEMANAGER->CenterFrameRender(m_image[eShoulder], m_center.x - 56, m_center.y - 16, 0, 0, eLayerUnderPlayer);
		IMAGEMANAGER->CenterFrameRender(m_image[eShoulder], m_center.x + 56, m_center.y - 16, 1, 0, eLayerUnderPlayer);
	}
}

void Colossus::Release()
{
	m_isDelete = true;
	m_handL->ObjectDelete();
	m_handR->ObjectDelete();
}

void Colossus::Attack(eObjectKinds kinds)
{

}

void Colossus::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eArrow:
		if (m_wakeup)
		{

		}
		else
		{
			m_ready = true;
			m_headImageIdx = 2;
			m_isOnHit = false;

			m_handL->SetImageFrame(3);
			m_handR->SetImageFrame(3);

			CAMERA->SetCameraMove(m_center, true, true, 5);
			CAMERA->SetCameraShaking(true, 5, 15);
		}
		break;
	default:
		break;
	}
}

void Colossus::SetFloor()
{
	SetPosition(800.f + 8, 920.f - TILE_SIZE);
	m_wakeup = false;
	m_isOnHit = true;
	m_ready = false;
	m_headImageIdx = 1;
}

void Colossus::SetActionScene()
{
	SetPosition(480.f, 288.f);
	if (m_isAlive) m_wakeup = true;
	m_isOnHit = true;
	m_ready = false;
	m_headImageIdx = 0;
}

Colossus::Colossus()
{
	m_center = { 800 + 8, 1918 - TILE_SIZE };
	m_wakeup = false;
	m_ready = false;
	m_sceneChangeAlpha = 0;

	m_handL = new ColossusHand(true);
	m_handR = new ColossusHand(false);
}

Colossus::~Colossus()
{
}


#pragma region 콜로서스 손
void ColossusHand::Init()
{
	if (m_leftHand)
	{
		m_image = IMAGEMANAGER->FindImage("ColossusHandLeft");
		//##그림자 터져
		m_shadow = IMAGEMANAGER->FindImage("ColossusHandShadowLeft");
		m_center.x = 760;
	}
	else
	{
		m_image = IMAGEMANAGER->FindImage("ColossusHandRight");
		m_shadow = IMAGEMANAGER->FindImage("colossusHandShadowRight");
		m_center.x = 856;
	}
}

void ColossusHand::Update()
{
	if (m_attack)
	{
		if (m_up)
		{
			HandUp();
		}
		else if (m_down)
		{
			HandDown();
		}
	}
	else if (m_comeback)
	{
		ComeBack();
	}
}

void ColossusHand::Render()
{
	eLayer layer = PLAYER->GetPointF().y > m_hitboxCenter.y ? eLayerUnderPlayer : eLayerUpperPlayer;
	if (m_attack) layer = eLayerUpperPlayer;

	IMAGEMANAGER->CenterFrameRender(m_image, m_hitboxCenter.x, m_hitboxCenter.y, m_imageFrame, 0, layer);
	IMAGEMANAGER->CenterFrameRender(m_shadow, m_attackCenter.x, m_attackCenter.y, m_imageFrame, 0, eLayerShadow);
}

void ColossusHand::Release()
{
}

void ColossusHand::Attack(eObjectKinds kinds)
{
}

void ColossusHand::Hit(eObjectKinds kinds)
{
	/*
	Do Nothing
	*/
}

void ColossusHand::HandUp()
{
	m_attackCenter = PLAYER->GetPointF();
	float moveDistanceH = (-sqrtf(m_actionTime) + sqrtf(m_actionTime + DELTA_TIME))
		* MY_UTIL::getDistance(m_center.x, m_center.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y - 80);
	float angleH = MY_UTIL::getAngle(m_hitboxCenter.x, m_hitboxCenter.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y - 80);
	m_hitboxCenter.x += cosf(angleH) * moveDistanceH;
	m_hitboxCenter.y -= sinf(angleH) * moveDistanceH;
	float moveDistanceA = (-sqrtf(m_actionTime) + sqrtf(m_actionTime + DELTA_TIME))
		* MY_UTIL::getDistance(m_center.x, m_center.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y);
	float angleA = MY_UTIL::getAngle(m_hitboxCenter.x, m_hitboxCenter.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y);
	m_attackCenter.x += cosf(angleA) * moveDistanceA;
	m_attackCenter.y -= sinf(angleA) * moveDistanceA;
	m_actionTime += DELTA_TIME;
	if (m_actionTime >= 1.f)
	{
		m_up = false;
		m_down = true;
		m_actionTime = 0;
		m_center = m_hitboxCenter;
		m_imageFrame = 0;
	}
}

void ColossusHand::HandDown()
{
	if (m_actionTime < 0.25f)
	{
		m_attackCenter = PLAYER->GetPointF();
		float moveDistance = (-sqrtf(m_actionTime) + sqrtf(m_actionTime + DELTA_TIME))
			* MY_UTIL::getDistance(m_center.x, m_center.y, m_attackCenter.x, m_attackCenter.y) / sqrtf(0.5f);
		float angle = MY_UTIL::getAngle(m_hitboxCenter.x, m_hitboxCenter.y, m_attackCenter.x, m_attackCenter.y);
		m_hitboxCenter.x += cosf(angle) * moveDistance;
		m_hitboxCenter.y = m_attackCenter.y - 80.f + 160.f * m_actionTime;
	}
	else if (m_actionTime < 0.5f)
	{
		m_hitboxCenter.y = m_attackCenter.y - 80.f + 160.f * m_actionTime;
		if (m_hitboxCenter.y >= m_attackCenter.y)
		{
			m_hitboxCenter.y = m_attackCenter.y;
			m_attack = false;
			m_down = false;
			m_comeback = true;
			m_isOnAttack = true;
			m_isOnHit = true;
			m_actionTime = 0.f;
			m_center = m_attackCenter;
		}
		else if (m_hitboxCenter.y >= m_attackCenter.y - 2.f)
		{
			m_isOnAttack = true;
			m_isOnHit = true;
		}
	}
	else
	{
		m_down = false;
		m_comeback = true;
		m_attack = false;
		m_actionTime = 0;
		m_center = m_hitboxCenter;
		m_imageFrame = 3;
	}
	m_actionTime += DELTA_TIME;
}

void ColossusHand::ComeBack()
{
	m_actionTime += DELTA_TIME;
	if (m_actionTime < 1.0f) return;

	float distance = m_moveSpeed * DELTA_TIME;
	float angle = MY_UTIL::getAngle(m_hitboxCenter.x, m_hitboxCenter.y, m_colossusCenter.x, m_colossusCenter.y);
	m_hitboxCenter.x += cosf(angle) * distance;
	m_hitboxCenter.y -= sinf(angle) * distance;
	if (MY_UTIL::getDistance(m_hitboxCenter.x, m_hitboxCenter.y, m_colossusCenter.x, m_colossusCenter.y) <= distance)
	{
		m_hitboxCenter = m_colossusCenter;
		m_center = m_hitboxCenter;
		m_comeback = false;
		m_imageFrame = 3;
		m_actionTime = 0.f;
	}

}

ColossusHand::ColossusHand(bool leftHand)
	:
	m_imageFrame(2),
	m_moveSpeed(15),
	m_actionTime(0),
	m_leftHand(leftHand),
	m_attack(false),
	m_up(false),
	m_down(false),
	m_comeback(false),
	m_colossusCenter({ 816, 928 })
{
	m_kinds = eMonsterProjectile;
	m_colossusCenter = { 800.f + 8, 920.f + 16 };

	m_center.x = leftHand ? 760 : 856;
	m_center.y = 968;
	m_hitboxCenter = m_attackCenter = m_center;

}

ColossusHand::~ColossusHand()
{
}
#pragma endregion