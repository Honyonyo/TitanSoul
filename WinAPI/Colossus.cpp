#include "stdafx.h"
#include "Colossus.h"

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
	m_hitboxRange = 80.f;

	m_handL = new ColossusHand(true);
	m_handR = new ColossusHand(false);
	m_handL->Init();
	m_handR->Init();
	OBJECTMANAGER->AddObject(m_handL);
	OBJECTMANAGER->AddObject(m_handR);
}

void Colossus::Update()
{
	if (KEYMANAGER->isOnceKeyDown('R')) 
	{
		m_isAlive = true; m_wakeup = false;
		m_handL->SetStart();
		m_handR->SetStart();
	}

	if (m_isAlive)
	{
		if (KEYMANAGER->isOnceKeyDown('M'))
		{
			return;
		}

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
			//피격포인트 반짝이기
			if (!(m_handL->GetIsOnDeffend() || m_handR->GetIsOnDeffend()))
			{
				m_hitpointEffectOn = true;
				m_hitpointEffectAlpha += 1 * DELTA_TIME;
			}
			else
			{
				m_hitpointEffectOn = false;
				m_hitpointEffectAlpha = 0;
			}

		}//end if wakeup
		else
		{
			if (m_ready)
			{
				if (m_sceneChangeAlpha > 1)
				{
					m_ready = false;
					m_wakeup = true;
					m_isOnHit = true;
					m_sceneChangeAlpha = NULL;
					m_headImageIdx = 0;
					CAMERA->SetCameraShakingOff();
					CAMERA->SetCameraMove(PLAYER->GetPointF(), true, false, 3);
					PLAYER->ResetPlayer();
					return;
				}
				m_sceneChangeAlpha += DELTA_TIME * 0.3;
			}//end if ready
			else
			{

			}
		}//end else wakeup
	}//end if alive
	//죽은상태에서 sCA가 값이 있을 경우
	else if (m_sceneChangeAlpha != NULL)
	{
		m_sceneChangeAlpha -= 0.2 * DELTA_TIME;
		if (m_sceneChangeAlpha < 0)
		{
			m_sceneChangeAlpha = NULL;
			SOUNDMANAGER->play("BGMFloor",1.f);
		}
	}
	//죽은상태에서 sCA = NULL인경우
	else
	{

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
		if (!m_isAlive)
		{
			if (m_sceneChangeAlpha != NULL)
			{
				IMAGEMANAGER->CenterFrameRender(m_image[eBody], m_center.x, m_center.y, 0, 0, eLayerUnderPlayer);
				IMAGEMANAGER->CenterFrameRender(m_image[eHead], m_center.x, m_center.y - 40, 3, 0, eLayerUnderPlayer);
				IMAGEMANAGER->CenterFrameRender(m_image[eShoulder], m_center.x - 56, m_center.y - 16, 0, 0, eLayerUnderPlayer);
				IMAGEMANAGER->CenterFrameRender(m_image[eShoulder], m_center.x + 56, m_center.y - 16, 1, 0, eLayerUnderPlayer);

				IMAGEMANAGER->DrawWhiteRect({ 0,0,WINSIZE_X, WINSIZE_Y }, eLayerTop, 1.f - m_sceneChangeAlpha);
			}
			else
			{
				IMAGEMANAGER->CenterFrameRender(m_image[eHead], 808, 1918, 1, 0, PLAYER->GetPointF().y > 1918 ? eLayerUnderPlayer : eLayerUpperPlayer);
			}
		}
	}//end if !wakeup
	else
	{
		IMAGEMANAGER->CenterFrameRender(m_image[eBody], m_center.x, m_center.y, 0, 0, eLayerUnderPlayer);
		IMAGEMANAGER->CenterFrameRender(m_image[eHead], m_center.x, m_center.y - 40, m_headImageIdx, 0, eLayerUnderPlayer);
		IMAGEMANAGER->CenterFrameRender(m_image[eShoulder], m_center.x - 56, m_center.y - 16, 0, 0, eLayerUnderPlayer);
		IMAGEMANAGER->CenterFrameRender(m_image[eShoulder], m_center.x + 56, m_center.y - 16, 1, 0, eLayerUnderPlayer);

		if (m_hitpointEffectOn)
		{
			int tmp = (int)m_hitpointEffectAlpha;
			float alpha = tmp % 2 ?
				m_hitpointEffectAlpha - tmp : 1 + tmp - m_hitpointEffectAlpha;
			IMAGEMANAGER->CenterFrameRender(IMAGEMANAGER->FindImage("ColossusBodyLight"), m_center.x, m_center.y, 0, 0, eLayerUnderPlayer, 1.f, 1.f, 0.f, alpha);
		}
	}//end if wakeup
}

void Colossus::Release()
{
	m_isDelete = true;
	m_handL->ObjectDelete();
	m_handR->ObjectDelete();
}

void Colossus::Attack(eObjectKinds kinds)
{
	if (kinds == ePlayer)
	{
		SOUNDMANAGER->play("DeathCrush", 1.f);
	}
}

void Colossus::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eArrow:
		cout << "콜로서스 화살에 충돌" << endl;
		if (m_wakeup)
		{
			m_sceneChangeAlpha = 1.f;
			m_wakeup = false;
			m_isAlive = false;
			SOUNDMANAGER->stop("BGMColosus");
			SOUNDMANAGER->play("Colossus_Roar",1.f);
			GAMEMANAGER->SetBossDead(eColossus);
		}
		else if (m_isAlive)
		{
			m_ready = true;
			m_headImageIdx = 2;
			m_isOnHit = false;
			m_hitboxRange = 8.f;

			m_handL->SetImageFrame(3);
			m_handR->SetImageFrame(3);
			SOUNDMANAGER->stop("BGMFloor");
			SOUNDMANAGER->play("BGMColosus",1.f);
			SOUNDMANAGER->play("Colossus_Rise",1.f);
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
	m_hitpointEffectAlpha = 0;
	m_hitpointEffectOn = false;
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
		m_shadow = IMAGEMANAGER->FindImage("ColossusHandShadowRight");
		m_center.x = 856;
	}
}

void ColossusHand::Update()
{
	if (KEYMANAGER->isToggleKey('M'))	 return;
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
	float shadowScale = 1.f - (m_attackCenter.y - m_hitboxCenter.y) * 0.00625;
	IMAGEMANAGER->CenterFrameRender(m_shadow, m_attackCenter.x, m_attackCenter.y, m_imageFrame, 0, eLayerShadow, shadowScale, shadowScale, 0.f, 0.5f);
}

void ColossusHand::Release()
{
}

void ColossusHand::Attack(eObjectKinds kinds)
{
}

void ColossusHand::Hit(eObjectKinds kinds)
{
	cout << "콜로서스 손 화살에 충돌" << endl;
	/*
	Do Nothing
	*/
}

void ColossusHand::HandUp()
{
	if (m_actionTime != 0)
	{
		float moveDistance = sqrtf(m_actionTime) * 
			MY_UTIL::getDistance
			(
				m_moveStartP.x, m_moveStartP.y, 
				PLAYER->GetPointF().x, PLAYER->GetPointF().y - 80
			) / sqrtf(1.50f);
		float angle = MY_UTIL::getAngle
		(
			m_moveStartP.x, m_moveStartP.y, 
			PLAYER->GetPointF().x, PLAYER->GetPointF().y - 80
		);
		m_hitboxCenter.x = m_moveStartP.x + cosf(angle) * moveDistance;
		m_hitboxCenter.y = m_moveStartP.y - sinf(angle) * moveDistance;
		m_attackCenter.x = m_hitboxCenter.x;
		m_attackCenter.y = m_hitboxCenter.y + m_actionTime * 160 / 3;
	}
	m_actionTime += DELTA_TIME;
	if (m_actionTime >= 1.5f)
	{
		m_up = false;
		m_down = true;
		m_actionTime = 0;
		m_moveStartP = m_attackCenter;
		m_center = m_hitboxCenter;
		m_imageFrame = 3;
	}
	cout << m_attackCenter.x << ", " << m_attackCenter.y << endl;
}

void ColossusHand::HandDown()
{
	if (m_actionTime < 0.25f)
	{
		float moveDistance = sqrtf(m_actionTime)
			* MY_UTIL::getDistance(m_moveStartP.x, m_moveStartP.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y)/*sqrtf(0.25f)*/;
		float angle = MY_UTIL::getAngle(m_moveStartP.x, m_moveStartP.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y);
		m_attackCenter.x = m_moveStartP.x + cosf(angle) * moveDistance;
		m_attackCenter.y = m_moveStartP.y - sinf(angle) * moveDistance;
		m_hitboxCenter.x = m_attackCenter.x;
		m_hitboxCenter.y = m_attackCenter.y - 80.f + 160.f * m_actionTime;
	}
	else if (m_actionTime > 0.5f&& m_actionTime<1.f)
	{
		m_hitboxCenter.y = m_attackCenter.y - 80.f + 160.f * m_actionTime;
		if (m_hitboxCenter.y >= m_attackCenter.y)
		{
			m_hitboxCenter.y = m_attackCenter.y;
			m_isOnAttack = true;
			m_isOnHit = true;

			if (!m_playHitEffectSound)
			{
				m_playHitEffectSound = true;
				string key = "Colossus_Hit";
				SOUNDMANAGER->play(key + (char)(RND->getInt(4) + 49), 1.f);
			}
			CAMERA->SetCameraShaking(true, 3, 32, 0.5f);
		}
		else if (m_hitboxCenter.y >= m_attackCenter.y - 2.f)
		{
			m_isOnAttack = true;
			m_isOnHit = true;

			if (!m_playHitEffectSound)
			{
				m_playHitEffectSound = true;
				string key = "Colossus_Hit";
				SOUNDMANAGER->play(key + (char)(RND->getInt(4) + 49), 1.f);
			}
			CAMERA->SetCameraShaking(true, 3, 32, 0.5f);
		}
	}
	else if (m_actionTime >= 2.f)
	{
		m_down = false;
		m_comeback = true;
		m_attack = false;
		m_actionTime = 0;
		m_center = m_hitboxCenter;
		m_imageFrame = 2;
	}
	m_actionTime += DELTA_TIME;
}

void ColossusHand::ComeBack()
{
	float distance = m_moveSpeed * DELTA_TIME;
	float angle = MY_UTIL::getAngle(m_hitboxCenter.x, m_hitboxCenter.y, m_colossusCenter.x, m_colossusCenter.y);
	m_hitboxCenter.x += cosf(angle) * distance;
	m_hitboxCenter.y -= sinf(angle) * distance;
	m_center = m_hitboxCenter;
	m_attackCenter = m_hitboxCenter;
	if (MY_UTIL::getDistance(m_hitboxCenter.x, m_hitboxCenter.y, m_colossusCenter.x, m_colossusCenter.y) <= distance)
	{
		m_hitboxCenter = m_colossusCenter;
		m_comeback = false;
		m_imageFrame = 2;
		m_actionTime = 0.f;
	}
}

void ColossusHand::PlaySoundEffect()
{
}

ColossusHand::ColossusHand(bool leftHand)
	:
	m_image(nullptr), m_shadow(nullptr),
	m_imageFrame(2),
	m_moveSpeed(64),
	m_actionTime(0),
	m_leftHand(leftHand),
	m_attack(false), m_up(false), m_down(false), m_comeback(false),
	m_playHitEffectSound(false),
	m_colossusCenter({ 816, 928 })
{
	m_kinds = eMonsterProjectile;
	m_colossusCenter = { (leftHand ? 800.f : 816.f), 920.f + 16 };
	SetStart();
}

ColossusHand::~ColossusHand()
{
}
#pragma endregion