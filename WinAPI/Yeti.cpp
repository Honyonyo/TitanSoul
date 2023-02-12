#include "stdafx.h"
#include "Yeti.h"
#include "Animation.h"
#include "PixelCollision.h"
#include "YetiProjectile.h"
void Yeti::Init()
{
	m_hitboxRange = 62.0f;
	m_sprite = IMAGEMANAGER->FindImage("YetiImageSheet");
	//그림자 scale 075
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

	m_pixelCollision = new PixelCollision(&m_center, 0, 16, 64, 64);
	m_pixelCollision->Init();

	GAMEMANAGER->GetBossState(eYeti, &m_isAlive, &m_center, &m_direction);

	SetAnimationFrame();
	SetStart();
}

void Yeti::Update()
{
	/// 
	/// 	동작을 시작하면
	/// 	해당 bitset을 on한다.
	/// 	isAction을 true한다
	///		동작 세팅을 한다. - 애니메이션 프레임 세팅, fps세팅 
	/// 	actionAni가 끝났는지 확인하고,
	/// 	안끝나면 현재 m_action에 따라 switch로 함수 호출한다.
	///		끝나면 해당 동작이 끝난것으로 표시해준다.
	/// 	다음 동작을 실행한다.
	/// 	queue가 비어있으면 SetPattern 실행

	eAction prevAction = m_action;
	eMoveDirection prevDirection = m_direction;
	int prevFrame = m_animation->GetNowFrameIdx();

	m_animation->FrameUpdate(TIMEMANAGER->getElapsedTime());

	SetCollUpdate(prevFrame);

	//자고있지 않을 때
	if (!m_isSleep)
	{
		m_isAction = m_animation->IsPlay();

		//여기에 동작 끝났는지 확인
		if (!m_isAction)
		{
			SetDirection();
			if (m_pattern.empty())
			{
				SetPattern();
			}

			switch (m_pattern.front())
			{
			case eRolling:
				SetRolling();
				cout << "SetRolling" << endl;
				break;
			case eThrow:
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
		m_isSleep = false;
	}
	if (KEYMANAGER->isOnceKeyDown('D'))
	{
		m_action = eDeath;
	}

	if (prevAction != m_action || prevDirection != m_direction &&m_action!=eRolling)
	{
		m_animation->SetPlayFrame(m_aniIndexArr[m_direction][m_action], false, 0);
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
	if (m_hitWall)
	{

	}
	else { IMAGEMANAGER->CenterFrameRender(m_shadow, m_shadowCenter.x, m_shadowCenter.y, 0, 0, 0.75f, 0, 0.5); }
	if (m_direction == eLeft || m_direction == eLeftDown || m_direction == eLeftUp)
	{
		IMAGEMANAGER->CenterAniRender(m_sprite, m_center.x, m_center.y, m_animation, true);
	}
	else { IMAGEMANAGER->CenterAniRender(m_sprite, m_center.x, m_center.y, m_animation); }
}

void Yeti::Release()
{
}

void Yeti::Attack(eObjectKinds kinds)
{
	switch (kinds)
	{
		case ePlayer : 
			cout << "예티 플레이어와 충돌" << endl;
			break;
		default:
			NULL;
	}
}

void Yeti::Hit(eObjectKinds kinds)
{
}

void Yeti::SetPattern()
{
	//int a = RND->getInt(3);
	//switch (a)
	//{
	//case 0:
	//	if (m_action == eThrow) { m_pattern.push(eReady); }
	//	m_pattern.push(eThrow);
	//	m_pattern.push(eThrow);
	//	m_pattern.push(eThrow);
	//	m_pattern.push(eThrow);
	//	break;
	//case 1:
	//	if (m_action == eRolling) { m_pattern.push(eReady); }
	//	a = RND->getInt(10);
	//	if (a < 1) a = 1;
	//	else if (a < 3) a = 2;
	//	else if (a < 7) a = 3;
	//	else a = 4;
	//	for (int i = 0; i < a; i++)
	//	{
	//		m_pattern.push(eRolling);
	//	}
	//	break;
	//case 2:
	//	m_pattern.push(eReady);
	//	break;
	//default:
	//	cout << "예티 패턴 넣기 실패" << endl;
	//}
	m_pattern.push(eRolling);
};

void Yeti::SetThrowSnowball()
{
	m_action = eThrow;
	m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eThrow]);
	m_animation->AniStart();
	m_animation->SetFPS(7);
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

}
void Yeti::Rolling()
{
	int nowFrameIndex = m_animation->GetNowFrameIdx();
	if (!m_isOnRollingLoop && !m_hitWall)
	{
		if (nowFrameIndex ==6)
		{
			m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eRollingLoop], true, nowFrameIndex - 6);
			m_animation->SetFPS(7);
			m_isOnRollingLoop = true;
		}
	}
	if (m_isOnRollingLoop)
	{
		if (m_pixelCollision->GetIsColl() && !m_hitWall)	//구르기중 충돌시 구르기점프 세팅하기
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
				cout << "예티 롤링 벽치고 각도계산 수상하다" << endl;
			}

			m_isOnAttack = false;
			m_hitWall = true;
			m_pixelCollision->ResetColl();
			m_rollingGravity = 278;
			m_rollingJumpSpeed = m_rollingDefJumpSpeed;
			m_rollingJumpMoveX = (m_rollingDest.x - m_center.x) / 2;	//초당 x이동거리 (총 이동시간 2)
			m_rollingJumpMoveY = (m_rollingDest.y - m_center.y) / 2;	//초당 y이동거리 (총 이동시간 2)

			IcicleDrop();
			//충돌하면
			// 타일 2~3장높이정도 뜬 다음
			//각도 말고 direction기준으로 반대방향으로 예티사이즈 2배정도지점에 착지, 마지막프레임 속도 초당 3정도로 조절해서 난이도 낮추기
		}
		if (m_hitWall)	//구르기 점프식
		{

			m_center.x += m_rollingJumpMoveX * DELTA_TIME;
			m_center.y += (m_rollingJumpMoveY-m_rollingJumpSpeed) * DELTA_TIME;
			m_rollingJumpSpeed -= m_rollingGravity*DELTA_TIME;

			//(점프로 구현하면 되겠다)
			//center점이 착지목표점까지의 직선을 기준으로 점프로 떠있음
			//if (m_center.x - m_rollingDest.x < 5 && m_center.x - m_rollingDest.x>5)

				//center점이 착지 목표점에 도착하면 m_hitWall을 끄고 Rolling마지막 프레임으로 돌린다. + 타격on, hitWall false
			if (MY_UTIL::getDistance(m_center.x, m_center.y, m_rollingDest.x, m_rollingDest.y) < 3)
			{
				m_center = m_rollingDest;
			m_isOnAttack = true;
			m_hitWall = false;
			m_isOnRollingLoop = false;
			m_animation->SetPlayFrame(m_aniIndexArr[m_direction][eRolling], false, m_aniIndexArr[m_direction][eRolling].size() - 1);
			m_animation->AniResume();
			}
			m_shadowCenter.x += m_rollingJumpMoveX * DELTA_TIME; 
			m_shadowCenter.y += m_rollingJumpMoveY * DELTA_TIME;
		}
		else//일반적인 구르기
		{
			m_center.x += cosf(m_rotRad) * m_rollingSpeed;
			m_center.y -= sinf(m_rotRad) * m_rollingSpeed;
		}
	}
}
void Yeti::IcicleDrop()
{
	//타일기준으로 박아버리면 어떨까?
	// coll타일 0인데에만 고드름생성가능하고 아닌데가 나오면 생성스탑
	//고드름간 최대거리 : 2고드름
	//고드름간 최소거리 : 1고드름
	//중심선 기준 편차 : 1고드름

	OBJECTMANAGER->AddObject<Icicle>()->Setting(PLAYER->GetPoint().x, PLAYER->GetPoint().y, 1);

}
void Yeti::Ready(int prevFrame)
{

};

void Yeti::SetCollUpdate(int aniIdx)
{
	switch (m_action)
	{
	case eIdle:
		//hitpoint잡아주고, attack off
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
		}
		break;
	default: m_isOnAttack = false;
	}
	if (KEYMANAGER->isOnceKeyDown('O'))
	{
		cout << "Yeti attackPoint " << m_attackCenter.x << ", " << m_attackCenter.y << endl;
		cout << "range" << m_attackRange << endl;
	}
};


void Yeti::SetAnimationFrame()
{
	if (m_isAlive)
	{
		for (int i = 0; i < eMoveDirectionNumber; i++)
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

		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eThrow);
		m_pattern.push(eRolling);
		m_pattern.push(eRolling);
	}
	m_action = eSit;
	m_rot = 0; m_rotRad = 0;
	m_isAction = false;
	m_animation->SetPlayFrame(m_aniIndexArr[eDown][eSit]);

	m_shadowCenter.x = m_center.x;
	m_shadowCenter.y = m_center.y + 45;
}

Yeti::Yeti()
	:m_rollingSpeed(10), m_maxSnowball(4), m_action(eSit),
	m_isAction(false), m_isSleep(true), m_isOnRollingLoop(false), m_hitWall(false),
	m_rollingJumpMoveX(0), m_rollingJumpMoveY(0),
	m_rot(0), m_rotRad(0),
	m_rollingJumpSpeed(0), m_rollingGravity(0), m_rollingDefJumpSpeed(278)
{
	m_direction = eDown;
	m_isOnHit = false;
	m_isOnAttack = false;
	m_kinds = eMonster;
};
Yeti::~Yeti()
{

};
