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
			CAMERA->SetCameraMove(m_center, true, true, 5);
			CAMERA->SetCameraShaking(true, 5, 15);
			m_headImageIdx = 2;
			m_isOnHit = false;
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

	m_handL = new ColossusHand(&m_center, true);
	m_handR = new ColossusHand(&m_center, false);
}

Colossus::~Colossus()
{
}


#pragma region 콜로서스 손
void ColossusHand::Init()
{
	//m_image[eHand] = IMAGEMANAGER->FindImage("ColossusHand");
	//m_image[eHandShadow] = IMAGEMANAGER->FindImage("ColossusHandShadow");
}

void ColossusHand::Update()
{
}

void ColossusHand::Render()
{

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
}

void ColossusHand::HandDown()
{
}

void ColossusHand::ComeBack()
{
}

ColossusHand::ColossusHand(D2D1_POINT_2F* colossusCenter, bool leftHand)
{
	m_kinds = eMonsterProjectile;
	m_colossusCenter = colossusCenter;
	m_leftHand = leftHand;
	if (leftHand)
	{
		m_image = IMAGEMANAGER->FindImage("ColossusHandLeft");
		m_center.x = colossusCenter->x - 80;
	}
	else
	{
		m_image = IMAGEMANAGER->FindImage("ColossusHandRight");
		m_center.x = colossusCenter->x + 80;
	}
	//	D2D1_POINT_2F m_center;	//오브젝트 중심점
	//	float m_hitboxRange;	//피격 범위
	//	D2D_POINT_2F m_hitboxCenter;	//피격범위의 중심점
	//	float m_attackRange;	//공격범위
	//	D2D1_POINT_2F m_attackCenter;		//공격범위의 중심점

}

ColossusHand::~ColossusHand()
{
}
#pragma endregion