#include "stdafx.h"
#include "Yeti.h"
#include "Animation.h"
#include "PixelCollision.h"
#include "YetiProjectile.h"
#include "BossShield.h"

void Yeti::Init()
{
	m_hitboxRange = 62.0f;
	m_sprite = IMAGEMANAGER->FindImage("YetiImageSheet");
	//�׸��� scale 075
	m_shadow = IMAGEMANAGER->FindImage("YetiShadow");

	m_animation = new Animation;
	m_animation->Init(m_sprite);
	m_animation->SetFPS(3);

	m_kinds = eMonster;
	m_direction = eDown;
	m_action = eSit;

	m_attackCenter.x = m_center.x;
	m_attackCenter.y = m_center.y + 16;
	m_attackRange = 32;
	m_hitboxRange = 10;

	m_pixelCollision = new PixelCollision(&m_center, 0, 16, 64, 64);
	m_pixelCollision->Init();

	m_bossShield = new BossShield*[4];
	for (int i = 0; i < m_shieldNum; i++)
	{
		m_bossShield[i] = new BossShield;
		m_bossShield[i]->Init();
		OBJECTMANAGER->AddObject(m_bossShield[i]);
	}
	m_bossShield[0]->Setting(this, &m_center, 0, 0, 16);
	m_bossShield[1]->Setting(this, &m_center, 0, 4, 16);
	m_bossShield[2]->Setting(this, &m_center, -10, 0, 24);
	m_bossShield[3]->Setting(this, &m_center, 10, 0, 24);

	GAMEMANAGER->GetBossState(eYeti, &m_isAlive, &m_center, &m_direction);
	PLAYER->SetSleepOnOff(false);

	SetAnimationFrame();
	SetStart();
}

void Yeti::Update()
{
	eAction prevAction = m_action;
	eMoveDirection prevDirection = m_direction;
	int prevFrame = m_animation->GetNowFrameIdx();

	m_animation->FrameUpdate(TIMEMANAGER->getElapsedTime());

	SetCollUpdate(prevFrame);
	if (m_action == eDeath && !m_animation->IsPlay()) return;

	if (KEYMANAGER->isToggleKey('M')) return;

	//�ڰ����� ���� ��
	if (m_wakeup)
	{
		m_isAction = m_animation->IsPlay();

		//���⿡ ���� �������� Ȯ��
		if (!m_isAction)
		{
			if (m_pattern.empty())
			{
				SetPattern();
			}

			switch (m_pattern.front())
			{
			case eRolling:
				SetDirection();
				SetRolling();
				cout << "SetRolling" << endl;
				break;
			case eThrow:			
				SetDirection();
				SetThrowSnowball();
				cout << "SetThrowSnowball" << endl;
				break;
			case eReady:
				SetReady();
				cout << "SetReady" << endl;
				break;
			default:
				NULL;
			}
			m_pattern.pop();
			m_isAction = true;
		}

		if (m_isAction)
		{
			switch (m_action)
			{
			case eRolling:
				Rolling();
				break;

			case eThrow:
				ThrowSnowball();
				break;

			case eReady:
				Ready(prevFrame);

			default:
				NULL;
			}
		}//end action
	}
	if (KEYMANAGER->isOnceKeyDown('Q'))
	{
		m_wakeup = true;
	}
	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		m_action = eDeath;
	}

	if (prevAction != m_action || prevDirection != m_direction && m_action != eRolling)
	{
		m_animation->SetPlayFrame(m_aniIndexArr[m_direction][m_action], false, 0);
		if (m_action == eDeath)
			m_animation->SetFPS(3);
		m_animation->AniStart();
	}

	if (!m_hitWall)
	{
		m_pixelCollision->Update();
		m_shadowCenter.x = m_center.x;
		m_shadowCenter.y = m_center.y + 45;
	}
}

void Yeti::Render()
{
	eLayer layer = (m_center.y + 48 > PLAYER->GetPointF().y) ? eLayerUpperPlayer : eLayerUnderPlayer;
	//��Ƽ
	if (m_direction == eLeft || m_direction == eLeftDown || m_direction == eLeftUp)
	{
		IMAGEMANAGER->CenterAniRender(m_sprite, m_center.x, m_center.y, m_animation, layer, true);
	}
	else { IMAGEMANAGER->CenterAniRender(m_sprite, m_center.x, m_center.y, m_animation, layer); }

	//�׸���
	if (m_action == eDeath && m_animation->GetNowFrameIdx() != 0)
	{
		//direction�� ��ǥ��� �ʿ�##
		IMAGEMANAGER->CenterFrameRender(m_shadow, m_shadowCenter.x, m_center.y + 20, 0, 0, eLayerShadow, 1.f, 1.f, 0, 0.5);
	}
	else
	{
		if (!m_hitWall)
		{
			IMAGEMANAGER->CenterFrameRender(m_shadow, m_shadowCenter.x, m_shadowCenter.y, 0, 0, eLayerShadow, 0.75f, 0.75, 0, 0.5);
		}
		else {}
	}
}

void Yeti::Release()
{
	for (int i = 0; i < 4; i++)
	{
		m_bossShield[i]->ObjectDelete();
	}
	m_isDelete = true;
}

void Yeti::Attack(eObjectKinds kinds)
{
	switch (kinds)
	{
	case ePlayer:
		SOUNDMANAGER->play("DeathImpact", 1.f);
		break;
	default:
		NULL;
	}
}

void Yeti::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eArrow:
		m_isAlive = false;
		m_action = eDeath;
		m_animation->SetPlayFrame(m_aniIndexArr[m_direction][m_action], false, 0);
		m_animation->AniStart();

		GAMEMANAGER->SetBossDead(eYeti, m_center, m_direction);

		cout << "��Ƽ ���" << endl;
		break;
	default:
		break;
	}
}

void Yeti::SetPattern()
{
	int a = RND->getInt(3);
	//int a = 2;
	switch (a)
	{
	case 0:
		if (m_action == eThrow) { m_pattern.push(eReady); }
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		break;
	case 1:
		if (m_action == eRolling) { m_pattern.push(eReady); }
		a = RND->getInt(10);
		if (a < 1) a = 1;
		else if (a < 3) a = 2;
		else if (a < 7) a = 3;
		else a = 4;
		for (int i = 0; i < a; i++)
		{
			m_pattern.push(eRolling);
		}
		break;
	case 2:
		m_pattern.push(eReady);
		break;
	default:
		cout << "��Ƽ ���� �ֱ� ����" << endl;
		break;
	}
};

void Yeti::SetThrowSnowball()
{
	m_action = eThrow;
	m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eThrow]);
	m_animation->AniStart();
	m_animation->SetFPS(7);

	//3�����ӿ��� render����, 4�����ӿ��� shot
	snowball = new Snowball(m_center.x, m_center.y - TILE_SIZE);
	snowball->Init();
	OBJECTMANAGER->AddObject(snowball);
	string key = "Yeti_SnowThrow";
	SOUNDMANAGER->play(key + (char)(RND->getInt(5) + 49), 1.f);

};
void Yeti::SetRolling()
{
	m_action = eRolling;
	m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eRolling]);
	m_animation->AniStart();
	m_animation->SetFPS(7);
};
void Yeti::SetReady()
{
	m_action = eReady;
	m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eReady]);
	m_animation->AniStart();
	m_animation->SetFPS(3);
};

void Yeti::ThrowSnowball()
{
	//�������� 3���϶� m_render==false �� SetRenderȣ��(Get�Լ� ���巯����)
	if (m_animation->GetNowFrameIdx() == 3)
	{
		if (!snowball->GetIsRender())
		{
			snowball->SetRender();
		}
	}
	//�������� 4���� �� shot snowball �� snowball = nullptr;
	if (m_animation->GetNowFrameIdx() == 4)
	{
		if (snowball != nullptr)
		{
			snowball->SetMove(PLAYER->GetPointF());
			snowball = nullptr;
		}
	}
}
void Yeti::Rolling()
{
	int nowFrameIndex = m_animation->GetNowFrameIdx();
	if (!m_isOnRollingLoop && !m_hitWall)
	{
		if (nowFrameIndex == 6)
		{
			m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eRollingLoop], true, nowFrameIndex - 6);
			m_animation->SetFPS(7);
			m_isOnRollingLoop = true;
			string key = "Yeti_Roll";
			SOUNDMANAGER->play(key + (char)(RND->getInt(5) + 49), 1.f);
		}
	}
	if (m_isOnRollingLoop)
	{
		if (m_pixelCollision->GetIsColl() && !m_hitWall)	//�������� �浹�� ���������� �����ϱ�
		{
			m_rollingHitWallPoint = m_center;
			switch (m_direction)
			{
			case eRight:
				m_direction = eLeft;
				m_rotRad = PI;
				m_rollingDest.x = m_center.x - 120;
				m_rollingDest.y = m_center.y;
				break;
			case eRightDown:
				m_direction = eLeftUp;
				m_rot = 225;
				m_rotRad = m_rot * DEG_TO_RAD;
				m_rollingDest.x = m_center.x + 120 * cosf(m_rotRad);
				m_rollingDest.y = m_center.y + 120 * sinf(m_rotRad);
				break;
			case eDown:
				m_direction = eUp;
				m_rot = 270;
				m_rotRad = m_rot * DEG_TO_RAD;
				m_rollingDest.x = m_center.x;
				m_rollingDest.y = m_center.y - 120;
				break;
			case eLeftDown:
				m_direction = eRightUp;
				m_rot = 315;
				m_rotRad = m_rot * DEG_TO_RAD;
				m_rollingDest.x = m_center.x + 120 * cosf(m_rotRad);
				m_rollingDest.y = m_center.y + 120 * sinf(m_rotRad);
				break;
			case eLeft:
				m_direction = eRight;
				m_rot = 0;
				m_rotRad = 0;
				m_rollingDest.x = m_center.x + 120;
				m_rollingDest.y = m_center.y;
				break;
			case eLeftUp:
				m_direction = eRightDown;
				m_rot = 45;
				m_rotRad = m_rot * DEG_TO_RAD;
				m_rollingDest.x = m_center.x + 120 * cosf(m_rotRad);
				m_rollingDest.y = m_center.y + 120 * sinf(m_rotRad);
				break;
			case eUp:
				m_direction = eDown;
				m_rot = 90;
				m_rotRad = m_rot * DEG_TO_RAD;
				m_rollingDest.x = m_center.x;
				m_rollingDest.y = m_center.y + 120;
				break;
			case eRightUp:
				m_direction = eLeftDown;
				m_rot = 135;
				m_rotRad = m_rot * DEG_TO_RAD;
				m_rollingDest.x = m_center.x + 120 * cosf(m_rotRad);
				m_rollingDest.y = m_center.y + 120 * sinf(m_rotRad);
				break;
			default:
				cout << "��Ƽ �Ѹ� ��ġ�� ������� �����ϴ�" << endl;
			}

			m_isOnAttack = false;
			m_hitWall = true;
			SetCollOnOff(false);
			m_pixelCollision->ResetColl();
			m_rollingGravity = 278;
			m_rollingJumpSpeed = m_rollingDefJumpSpeed;
			m_rollingJumpMoveX = (m_rollingDest.x - m_center.x) / 2;	//�ʴ� x�̵��Ÿ� (�� �̵��ð� 2)
			m_rollingJumpMoveY = (m_rollingDest.y - m_center.y) / 2;	//�ʴ� y�̵��Ÿ� (�� �̵��ð� 2)

			string key = "Yeti_YetiBallImpact";
			SOUNDMANAGER->play(key + (char)(RND->getInt(5) + 49), 1.f);

			CAMERA->SetCameraShaking(true, 3, 32, 0.5f);

			IcicleDrop();
			//�浹�ϸ�
			// Ÿ�� 2~3��������� �� ����
			//���� ���� direction�������� �ݴ�������� ��Ƽ������ 2������������ ����, ������������ �ӵ� �ʴ� 3������ �����ؼ� ���̵� ���߱�
		}
		if (m_hitWall)	//������ ������
		{

			m_center.x += m_rollingJumpMoveX * DELTA_TIME;
			m_center.y += (m_rollingJumpMoveY - m_rollingJumpSpeed) * DELTA_TIME;
			m_rollingJumpSpeed -= m_rollingGravity * DELTA_TIME;

			//(������ �����ϸ� �ǰڴ�)
			//center���� ������ǥ�������� ������ �������� ������ ������
			//if (m_center.x - m_rollingDest.x < 5 && m_center.x - m_rollingDest.x>5)

				//center���� ���� ��ǥ���� �����ϸ� m_hitWall�� ���� Rolling������ ���������� ������. + Ÿ��on, hitWall false
			if (m_rollingJumpSpeed < 0)
			{
				if (MY_UTIL::getDistance(m_center.x, m_center.y, m_rollingDest.x, m_rollingDest.y) < 3)
				{
					m_center = m_rollingDest;
					m_isOnAttack = true;
					m_hitWall = false;
					SetCollOnOff(true);
					m_isOnRollingLoop = false;
					m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eRolling], false, m_aniIndexArr[m_direction][eRolling].size() - 1);
					m_animation->AniResume();

					string key = "Yeti_YetiLand";
					SOUNDMANAGER->play(key + (char)(RND->getInt(5) + 49), 1.f);

				}
			}
			m_shadowCenter.x += m_rollingJumpMoveX * DELTA_TIME;
			m_shadowCenter.y += m_rollingJumpMoveY * DELTA_TIME;
		}
		else//�Ϲ����� ������
		{
			m_center.x += cosf(m_rotRad) * m_rollingSpeed;
			m_center.y -= sinf(m_rotRad) * m_rollingSpeed;
		}
	}
}
void Yeti::IcicleDrop()
{
	//Ÿ�ϱ������� �ھƹ����� ���?
	// collŸ�� 0�ε����� ��帧���������ϰ� �ƴѵ��� ������ ������ž
	//��帧�� �ִ�Ÿ� : 2��帧
	//��帧�� �ּҰŸ� : 1��帧
	//�߽ɼ� ���� ���� : 1��帧

	//�÷��̾���� ������ y��� ���� ����������� ������
	D2D1_POINT_2F playerpoint = PLAYER->GetPointF();
	float slop;
	float tmp = (m_center.x > playerpoint.x) ? -2 : 2;
	if (playerpoint.x - m_center.x < TILE_SIZE)
	{
		slop = 0;
	}
	else
	{
		slop = (m_center.y - playerpoint.y) / (m_center.x - playerpoint.x);
	}
	int i = 1;
	POINT dropCenter = { 8 + (m_center.x) / TILE_SIZE * TILE_SIZE, 8 + (m_center.y / TILE_SIZE) * TILE_SIZE };
	while (i < 7)
	{
		dropCenter.x += (tmp + RND->getfloat(1.6) - 0.8) * TILE_SIZE;
		dropCenter.y += (slop + RND->getfloat(1.6) - 0.8) * TILE_SIZE * 2;
		RECT rc = { 0,0,SCENEMANAGER->GetCurrentSceneWidth(), SCENEMANAGER->GetCurrentSceneHeight() };
		if (!PtInRect(&rc, dropCenter))
		{
			i++; continue;
		}
		if (SCENEMANAGER->GetCurrentScenePixel()[dropCenter.x][dropCenter.y] != RGB(0, 0, 0))
		{
			break;
		}
		else
		{
			OBJECTMANAGER->AddObject<Icicle>()->Setting(dropCenter.x, dropCenter.y, i % 3);
			i++;
		}

	}

}
void Yeti::Ready(int prevFrame)
{
	if (prevFrame != m_animation->GetNowFrameIdx())
	{
		if(m_animation->GetNowFrameIdx() == 2)
			SetDirection();
	}
};

void Yeti::SetCollUpdate(int aniIdx)
{

	//0�Ӹ� 1���� 2�޾ 3�������
	//���⺰�� shield �ٸ�
	//rolling�� ���������̹Ƿ� ���� action����ġ���� �ٽ� ����

	switch (m_direction)
	{
	case eRight:
		m_bossShield[0]->Setting(10, -6);
		m_bossShield[1]->Setting(-3, 1);
		m_bossShield[2]->Setting(0, -3);
		m_bossShield[3]->Setting(0, -3);
		break;
	case eLeft:
		m_bossShield[0]->Setting(-10, -6);
		m_bossShield[1]->Setting(+3, 1);
		m_bossShield[2]->Setting(0, -3);
		m_bossShield[3]->Setting(0, -3);
		break;
	case eUp:
	case eDown:
		m_bossShield[0]->Setting(-3, -7);
		m_bossShield[1]->Setting(-3, 1);
		m_bossShield[2]->Setting(-13, -3);
		m_bossShield[3]->Setting(7, -3);
		break;
	case eRightDown:
		m_bossShield[0]->Setting(5, -5);
		m_bossShield[1]->Setting(-3, 1);
		m_bossShield[2]->Setting(-9, -3);
		m_bossShield[3]->Setting(7, -6);
		break;
	case eLeftDown:
		m_bossShield[0]->Setting(-5, -5);
		m_bossShield[1]->Setting(+3, 1);
		m_bossShield[2]->Setting(+9, -3);
		m_bossShield[3]->Setting(-7, -6);
		break;
	case eLeftUp:
		m_bossShield[0]->Setting(0, -5);
		m_bossShield[1]->Setting(+3, 1);
		m_bossShield[2]->Setting(+8, -5);
		m_bossShield[3]->Setting(-2, -3);
		break;
	case eRightUp:
		m_bossShield[0]->Setting(0, -5);
		m_bossShield[1]->Setting(-3, 1);
		m_bossShield[2]->Setting(-8, -5);
		m_bossShield[3]->Setting(2, -3);
		break;
	default:
		break;
	}

	switch (m_action)
	{
	case eIdle:
	case eReady:
	case eThrow:
		//hitpoint����ְ�, attack off
		m_isOnAttack = false;
		m_isOnHit = true;
		break;
	case eRolling:
		if (m_isOnRollingLoop)
		{
			m_isOnAttack = true;
			m_isOnHit = false;

			m_attackCenter.x = m_center.x;
			m_attackCenter.y = m_center.y + 16;
			m_bossShield[0]->Setting(0, 0);
		}
		break;
	default: m_isOnAttack = false;
	}
	if (KEYMANAGER->isOnceKeyDown('O'))
	{
		cout << "Yeti\nattackPoint " << m_attackCenter.x << ", " << m_attackCenter.y << endl;
		cout << "range" << m_attackRange << endl;
		cout << "hitboxPoint " << m_hitboxCenter.x << ", " << m_hitboxCenter.y << endl;
		cout << "range" << m_hitboxRange << endl;
	}

	switch (m_direction)
	{
	case eRight:
		m_hitboxCenter.x = m_center.x - 16;
		m_hitboxCenter.y = m_center.y + 22;
		break;
	case eUp:
		m_hitboxCenter.x = m_center.x;
		m_hitboxCenter.y = m_center.y + 22;
		break;
	case eLeft:
		m_hitboxCenter.x = m_center.x + 16;
		m_hitboxCenter.y = m_center.y + 22;
		break;
	case eDown:
	case eRightDown:
	case eLeftDown:
		m_isOnHit = false;
		break;
	case eLeftUp:
		m_hitboxCenter.x = m_center.x + 8;
		m_hitboxCenter.y = m_center.y + 22;
		break;
	case eRightUp:
		m_hitboxCenter.x = m_center.x - 8;
		m_hitboxCenter.y = m_center.y + 22;
		break;
	default:
		break;
	}
}

void Yeti::SetAnimationFrame()
{
	if (m_isAlive)
	{
		for (int i = 0; i < eMoveDirectionNumCount; i++)
		{
			m_aniIndexArr[i][eSit].push_back(73);
		}
		for (int i = 0; i < 10; i++)
		{
			m_aniIndexArr[eRight][eRolling].push_back(i);
			m_aniIndexArr[eLeft][eRolling].push_back(i);
			m_aniIndexArr[eUp][eRolling].push_back(i + 10);
			m_aniIndexArr[eDown][eRolling].push_back(i + 20);
			m_aniIndexArr[eRightUp][eRolling].push_back(i + 30);
			m_aniIndexArr[eLeftUp][eRolling].push_back(i + 30);
			m_aniIndexArr[eRightDown][eRolling].push_back(i + 40);
			m_aniIndexArr[eLeftDown][eRolling].push_back(i + 40);
		}
		for (int i = 0; i < 3; i++)
		{
			m_aniIndexArr[eRight][eIdle].push_back(50 + i);
			m_aniIndexArr[eLeft][eIdle].push_back(50 + i);
			m_aniIndexArr[eUp][eIdle].push_back(i + 60);
			m_aniIndexArr[eDown][eIdle].push_back(i + 70);
			m_aniIndexArr[eRightUp][eIdle].push_back(i + 80);
			m_aniIndexArr[eLeftUp][eIdle].push_back(i + 80);
			m_aniIndexArr[eRightDown][eIdle].push_back(i + 90);
			m_aniIndexArr[eLeftDown][eIdle].push_back(i + 90);

			m_aniIndexArr[eRight][eReady].push_back(i);
			m_aniIndexArr[eLeft][eReady].push_back(i);
			m_aniIndexArr[eUp][eReady].push_back(i + 10);
			m_aniIndexArr[eDown][eReady].push_back(i + 20);
			m_aniIndexArr[eRightUp][eReady].push_back(i + 30);
			m_aniIndexArr[eLeftUp][eReady].push_back(i + 30);
			m_aniIndexArr[eRightDown][eReady].push_back(i + 40);
			m_aniIndexArr[eLeftDown][eReady].push_back(i + 40);

			m_aniIndexArr[eRight][eRollingLoop].push_back(i + 6);
			m_aniIndexArr[eLeft][eRollingLoop].push_back(i + 6);
			m_aniIndexArr[eUp][eRollingLoop].push_back(i + 16);
			m_aniIndexArr[eDown][eRollingLoop].push_back(i + 26);
			m_aniIndexArr[eRightUp][eRollingLoop].push_back(i + 36);
			m_aniIndexArr[eLeftUp][eRollingLoop].push_back(i + 36);
			m_aniIndexArr[eRightDown][eRollingLoop].push_back(i + 46);
			m_aniIndexArr[eLeftDown][eRollingLoop].push_back(i + 46);

			m_aniIndexArr[eRight][eDeath].push_back(54 + i);
			m_aniIndexArr[eLeft][eDeath].push_back(54 + i);
			m_aniIndexArr[eUp][eDeath].push_back(i + 64);
			m_aniIndexArr[eDown][eDeath].push_back(i + 74);
			m_aniIndexArr[eRightUp][eDeath].push_back(i + 84);
			m_aniIndexArr[eLeftUp][eDeath].push_back(i + 84);
			m_aniIndexArr[eRightDown][eDeath].push_back(i + 94);
			m_aniIndexArr[eLeftDown][eDeath].push_back(i + 94);
		}

		for (int i = 0; i < 6; i++)
		{
			m_aniIndexArr[eRight][eThrow].push_back(i);
			m_aniIndexArr[eLeft][eThrow].push_back(i);
			m_aniIndexArr[eUp][eThrow].push_back(i + 10);
			m_aniIndexArr[eDown][eThrow].push_back(i + 20);
			m_aniIndexArr[eRightUp][eThrow].push_back(i + 30);
			m_aniIndexArr[eLeftUp][eThrow].push_back(i + 30);
			m_aniIndexArr[eRightDown][eThrow].push_back(i + 40);
			m_aniIndexArr[eLeftDown][eThrow].push_back(i + 40);
		}
	}
	else
	{
		for (int i = 0; i < eActionNumCount; i++)
		{
			m_aniIndexArr[eRight][i].push_back(56 + i);
			m_aniIndexArr[eLeft][i].push_back(56 + i);
			m_aniIndexArr[eUp][i].push_back(66 + i);
			m_aniIndexArr[eDown][i].push_back(76 + i);
			m_aniIndexArr[eRightUp][i].push_back(86 + i);
			m_aniIndexArr[eLeftUp][i].push_back(86 + i);
			m_aniIndexArr[eRightDown][i].push_back(96 + i);
			m_aniIndexArr[eLeftDown][i].push_back(96 + i);
		}
	}
};

void Yeti::SetDirection()
{
	m_rotRad = MY_UTIL::getAngle(m_center.x, m_center.y, PLAYER->GetPoint().x, PLAYER->GetPoint().y);
	m_rot = m_rotRad * RAD_TO_DEG;
	if (m_rot < 22.5f) m_direction = eRight;
	else if (m_rot < 67.5f) m_direction = eRightUp;
	else if (m_rot < 112.5f) m_direction = eUp;
	else if (m_rot < 157.5) m_direction = eLeftUp;
	else if (m_rot < 202.5) m_direction = eLeft;
	else if (m_rot < 247.5) m_direction = eLeftDown;
	else if (m_rot < 292.5) m_direction = eDown;
	else if (m_rot < 337.5) m_direction = eRightDown;
	else m_direction = eRight;
};

void Yeti::SetStart()
{
	if (m_isAlive)
	{
		m_center = { 496,480 };
		m_direction = eDown;
		m_action = eSit;

		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eRolling);
		m_pattern.push(eRolling);
	}
	m_rot = 0; m_rotRad = 0;
	m_isAction = false;
	m_animation->SetPlayFrame(m_aniIndexArr[m_direction][m_action], false, (int)(m_isAlive ? 0 : 2));

	m_shadowCenter.x = m_center.x;
	m_shadowCenter.y = m_center.y + 45;
}

Yeti::Yeti()
	:m_rollingSpeed(7), m_maxSnowball(4), m_action(eSit),
	m_isAction(false), m_isOnRollingLoop(false), m_hitWall(false),
	m_rollingJumpMoveX(0), m_rollingJumpMoveY(0),
	m_rot(0), m_rotRad(0),
	m_rollingJumpSpeed(0), m_rollingGravity(0), m_rollingDefJumpSpeed(278),
	snowball(nullptr)
{
	m_direction = eDown;
	m_isOnHit = false;
	m_isOnAttack = false;
	m_wakeup = false;
	m_shieldNum = 4;
	m_kinds = eMonster;
};
Yeti::~Yeti()
{

};
