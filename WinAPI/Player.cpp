#include "stdafx.h"
#include "Player.h"
#include "Camera.h"
#include "Animation.h"
#include "PixelCollision.h"

void Player::Init()
{
	m_Image = IMAGEMANAGER->FindImage("PlayerImageSheet");
	m_bowImage = IMAGEMANAGER->FindImage("BowImageSheet");

	m_animation = new Animation;
	m_animation->Init(m_Image);
	SetAnimationFrame();

	m_arrow = new Arrow(m_animation, &m_center);
	m_arrow->Init();
	OBJECTMANAGER->AddObject(m_arrow);

	m_pixelCollision = new PixelCollision(&m_center, 0, 4, 4, 4);
	m_pixelCollision->Init();

	m_inGround = false;
	m_direction = eDown;
	m_state = eIdle;
	//씬에서 플레이어 위치 초기값을 뱉어주면 그거로 세팅하기
	//커런트씬과 넥스트씬을 비교해서 넥스트씬에 switch로 플레이어위치초기값 조건달아서 뱉어주기
	m_animation->SetPlayFrame(m_aniIndexArr[1][m_direction][m_state], true);

	m_kinds = ePlayer;
	m_isAlive = true;
	m_isOnHit = true;
	m_isOnAttack = false;
	m_hitboxCenter = m_center;
	m_hitboxCenter.y = m_center.y - 4;
	m_hitboxRange = 4;
};
void Player::Update()
{
	//플레이어 죽으면 동작 안하기
	if (!m_isAlive && !m_animation->IsPlay()) return;
	if (m_sleep)return;

	m_aniChange = false;

	/// X키를 누르고 구르기동작중인 경우에는 구르기는 논캔슬이라 방향전환도 안되고, state변경도 안된다.
	/// <state변경에관하여>
	/// 시작시 state를 prevState에 저장한 후 기본인 idle로 바꾸고
	/// Update에서 무언가 커멘드가 들어갔을 경우 state를 해당 커멘드로 바꾼다.
	/// prevState와 state가 같으면 같은 동작을 이어서 하는것이므로 direction만 바꾼 후 프레임 유지하고 애니메이션 출력,
	/// 아닌경우 동작이 아예 바뀐것이므로 direction에 해당하는 새 state애니메이션을 0번째 프레임부터 출력한다.
	/// </state변경에관하여>
	/// <direction변경에관하여>
	/// 화살표키를 누르면 방향이 변하는데,
	/// C키를 안누르고있으면 걷거나 뛰면서 방향이 전환된다.
	/// arrow가 있을 경우 C키를 누르고있으면 쏠 방향 각도를 바꾸는 모드로 들어가고, 없으면 끌어당김모드인데 방향전환 불가하다.
	/// </direction변경에관하여>

	int frameIndex = 0;
	int prevAniIndex = m_animation->GetNowFrameIdx();
	eMoveDirection prevDirction = m_direction;
	eState prevState = m_state;
	if (!m_animation->IsPlay())
	{
		m_state = eIdle;
		m_nonCansleAction = false;
	}

	//C키 누르면 카메라 확대되기
	//화살 있을때 - 발사모션 / 화살 없을때 - 회수모션
	if (KEYMANAGER->isStayKeyDown('C'))
	{
		m_nonCansleAction = true;
		CAMERA->SetScaleIncease(true);
		if (m_arrow->GetIsShooted())
		{
			m_state = eCall;
			if (!m_arrow->GetIsRetrieved())
			{
				m_arrow->SetSpeed(0);
			}
			m_arrow->Retrieve();
		}
		else if (m_arrow->GetIsReady())
		{
			m_state = eShotting;
			m_arrow->DrawBow(m_direction);
		}

	}//end if 'C'		선형보간 - 이동하는 목표점을 향해 가야할 경우 유용(난 필요 없을듯?)
	else if (!m_nonCansleAction)//C안누르거나 구르기상태 아닌 경우에만 MOVE가능
	{
		if (m_inGround)
			if (m_state != eRolling && m_state != eRollingFail)
				Move();
	}//end if not 'C'
	Rolling();

	//C키 떼면 카메라 다시 돌아오기
	if (KEYMANAGER->isOnceKeyUp('C'))
	{
		CAMERA->SetScaleIncease(false);

		if (m_arrow->GetIsDrawed())
		{
			m_arrow->ShotArrow();
		}
		else
		{
			m_arrow->SetIsReady(true);
		}
		m_nonCansleAction = false;
	}

	if (m_direction != prevDirction)
	{
		m_aniChange = true;
	}
	if (m_state != prevState)
	{
		m_aniChange = true;
		m_soundeffectCount = 0;
		if (m_direction != prevDirction)
		{
			frameIndex = m_animation->GetNowFrameIdx();
		}
	}

	m_hitboxCenter = m_center;
	CoordSetting();
	if (m_tileSpec == eTileWater || m_tileSpec == eTileWater2)
	{
		m_aniChange = m_inGround ? true : m_aniChange;
		m_inGround = false;
	}
	else
	{
		m_aniChange = m_inGround ? m_aniChange : true;
		m_inGround = true;
	}

	if (m_aniChange)
	{
		if (m_state == eWalk || m_state == eDash)
		{
			m_animation->SetPlayFrame(m_aniIndexArr[m_inGround][m_direction][m_state], true, frameIndex);
		}
		else
		{
			m_animation->SetPlayFrame(m_aniIndexArr[m_inGround][m_direction][m_state], false, frameIndex);
		}
		if (!m_animation->IsPlay()) m_animation->AniStart();
		if (m_state == eRolling)
			m_animation->SetFPS(6);
		else
			m_animation->SetFPS(3);
	}
	m_animation->FrameUpdate(TIMEMANAGER->getElapsedTime());
	PlaySoundEffect(prevAniIndex, m_animation->GetNowFrameIdx());

	m_pixelCollision->Update();
};
void Player::Render()
{
	//그림자 렌더
	//IMAGEMANAGER->CenterFrameRender(m_Image, );

	IMAGEMANAGER->CenterAniRender(m_Image, m_center.x, m_center.y, m_animation, eLayerPlayer);
	IMAGEMANAGER->CenterAniRender(m_bowImage, m_center.x, m_center.y, m_animation, eLayerPlayer);
};
void Player::Release()
{
	SAFE_DELETE(m_animation);
};

void Player::Attack(eObjectKinds kinds)
{

};
void Player::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eMonster:
		m_isAlive = false;
		m_nonCansleAction = true;
		m_state = eDeath;
		m_animation->SetPlayFrame(m_aniIndexArr[m_inGround][m_direction][eDeath], false, 0);
		m_animation->AniStart();
		break;
	default:
		NULL;
	}
};

void Player::Move()
{
	m_state = eIdle;

	float speed = (KEYMANAGER->isStayKeyDown('X')) ? m_dashSpeed : m_moveSpeed;
	bool isDash = (speed == m_dashSpeed) ? true : false;

	eMoveDirection prevDirection = m_direction;

	m_directionKey.reset();

	/// <summary>
	/// 화살표 키를 눌렀을 때 : 방향전환
	/// 뗐을 때 : 다른 키를 누르고있다면 : 방향전환 ->어떻게 판별? state가 idle이 아닌 경우로 판별하기
	/// 뗐을 때 : 다른 키를 누르는게 없다면 : 키 떼기 직전 방향을 바라보고 멈추기 ->state가 idle인 경우
	/// </summary>
	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (!m_pixelCollision->GetLeftColl()) m_center.x -= speed;
		m_directionKey[0] = 1;
		m_directionKey[2] = 0;
		m_state = isDash ? eDash : eWalk;
		if (m_tileSpec > eTileFuncStart)
		{
			switch (m_tileSpec)
			{
			case eTileStaireLeft:
				m_center.y -= speed;
				break;
			case eTileStaireRight:
				m_center.y += speed;
				break;
			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (!m_pixelCollision->GetTopColl())	m_center.y -= speed;
		if (m_tileSpec == eTileStaireDown || m_tileSpec == eTileStaireUp) m_center.y += speed / 2;
		m_directionKey[1] = 1;
		m_directionKey[3] = 0;
		m_state = isDash ? eDash : eWalk;
	}
	if (KEYMANAGER->isStayKeyDown(VK_RIGHT))
	{
		if (!m_pixelCollision->GetRightColl()) m_center.x += speed;
		m_directionKey[0] = 0;
		m_directionKey[2] = 1;
		m_state = isDash ? eDash : eWalk;
		if (m_tileSpec > eTileFuncStart)
		{
			switch (m_tileSpec)
			{
			case eTileStaireLeft:
				m_center.y += speed;
				break;
			case eTileStaireRight:
				m_center.y -= speed;
				break;
			}
		}
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (!m_pixelCollision->GetBottomColl()) m_center.y += speed;
		if (m_tileSpec == eTileStaireDown || m_tileSpec == eTileStaireUp) m_center.y -= speed / 2;
		m_directionKey[1] = 0;
		m_directionKey[3] = 1;
		m_state = isDash ? eDash : eWalk;
	}
	if (m_state != eIdle)
	{
		DirectionSetting();
	}

};

void Player::Rolling()
{
	if (!m_nonCansleAction)
	{
		if (KEYMANAGER->isOnceKeyDown('X'))
		{
			m_state = eRolling;
			m_aniChange = true;
			m_nonCansleAction = true;
		}
	}

	if (m_state == eRolling)
	{
		short a = 1;
		if (m_pixelCollision->GetIsColl())
		{
			m_aniChange = true;
			m_state = eRollingFail;
			a = -1;
		}//end pixcelColl
		float speed = a * m_dashSpeed;
		switch (m_direction)
		{
		case eRight:
			m_center.x += a * m_dashSpeed;
			if (m_tileSpec == eTileStaireLeft) m_center.y += speed;
			if (m_tileSpec == eTileStaireRight) m_center.y -= speed;
			break;
		case eUp:
			m_center.y -= speed;
			if (m_tileSpec == eTileStaireDown || m_tileSpec == eTileStaireUp) m_center.y += speed / 2;
			break;
		case eLeft:
			m_center.x -= speed;
			if (m_tileSpec == eTileStaireLeft) m_center.y -= speed;
			if (m_tileSpec == eTileStaireRight) m_center.y += speed;
			break;
		case eDown:
			m_center.y += speed;
			if (m_tileSpec == eTileStaireDown || m_tileSpec == eTileStaireUp) m_center.y -= speed / 2;
			break;
		case eRightDown:
			m_center.x += speed;
			m_center.y += speed;
			if (m_tileSpec == eTileStaireLeft) m_center.y += speed;
			if (m_tileSpec == eTileStaireRight) m_center.y -= speed;
			break;
		case eLeftDown:
			m_center.x -= speed;
			m_center.y += speed;
			if (m_tileSpec == eTileStaireLeft) m_center.y -= speed;
			if (m_tileSpec == eTileStaireRight) m_center.y += speed;
			break;
		case eLeftUp:
			m_center.x -= speed;
			m_center.y -= speed;
			if (m_tileSpec == eTileStaireLeft) m_center.y -= speed;
			if (m_tileSpec == eTileStaireRight) m_center.y += speed;
			break;
		case eRightUp:
			m_center.x += speed;
			m_center.y -= speed;
			if (m_tileSpec == eTileStaireLeft) m_center.y += speed;
			if (m_tileSpec == eTileStaireRight) m_center.y -= speed;
			break;
		default:
			break;
		}//end noColl
	}//end stateRolling
};

void Player::DirectionSetting()
{
	int direction = m_directionKey.to_ulong();

	//bitset은 리틀엔디안으로 가는가보다..........  ㅠㅠ
	//L1000 T0100 R0010 B0001
	switch (direction)
	{
	case 1:
		m_direction = eLeft;
		break;
	case 2:
		m_direction = eUp;
		break;
	case 3:
		m_direction = eLeftUp;
		break;
	case 4:
		m_direction = eRight;
		break;
	case 6:
		m_direction = eRightUp;
		break;
	case 8:
		m_direction = eDown;
		break;
	case 9:
		m_direction = eLeftDown;
		break;
	case 12:
		m_direction = eRightDown;
		break;
	default:
		NULL;
	}
}


void Player::SetAnimationFrame()
{
	for (int i = 0; i < eMoveDirectionNumCount; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_aniIndexArr[1][i][eWalk].push_back(j + m_animation->GetFrameNumWidth() * i);
			m_aniIndexArr[1][i][eRolling].push_back(j + 6 + m_animation->GetFrameNumWidth() * i);
		}
		for (int j = 0; j < 8; j++)
		{
			m_aniIndexArr[0][i][eWalk].push_back(j + m_animation->GetFrameNumWidth() * (i + 8));
			m_aniIndexArr[0][i][eDash].push_back(j + m_animation->GetFrameNumWidth() * (i + 8));
			m_aniIndexArr[0][i][eIdle].push_back(j + m_animation->GetFrameNumWidth() * (i + 8));
		}
		for (int j = 0; j < 12; j++)
		{
			m_aniIndexArr[1][i][eDash].push_back(j + ((8 + i) * m_animation->GetFrameNumWidth()) + 8);
		}
		m_aniIndexArr[1][i][eShotting].push_back(13 + m_animation->GetFrameNumWidth() * i);
		m_aniIndexArr[1][i][eCall].push_back(14 + m_animation->GetFrameNumWidth() * i);
		m_aniIndexArr[1][i][eIdle].push_back(i * m_animation->GetFrameNumWidth());
		m_aniIndexArr[1][i][eRollingSuccess].push_back(12 + m_animation->GetFrameNumWidth() * i);
		m_aniIndexArr[1][i][eRollingFail].push_back(12 + m_animation->GetFrameNumWidth() * i);
		m_aniIndexArr[1][i][eDeath].push_back(12 + m_animation->GetFrameNumWidth() * i);
	}

	for (int i = 0; i < m_aniIndexArr[1][eRight][eWalk].size(); i++)
		m_animation->SetFPS(3);
	m_animation->AniStart();
}

void Player::PlaySoundEffect(int prevAniIdx, int nowAniIdx)
{
	//if (m_state != eWalk && m_state != eDash) return;
	//if (prevAniIdx == nowAniIdx) return;

	switch (m_state)
	{
	case eDash:
	case eWalk:
		if (prevAniIdx != nowAniIdx)
		{
			if (m_state != eWalk || nowAniIdx % 2 == 0)
			{
				char num = m_soundeffectCount + 48;
				//발자국소리 효과음 key값 조합
				string tmp;
				switch (m_tileSpec)
				{
				case eTileGrass:
					tmp = "GrassStep";
					tmp += num;
					break;

				case eTileStone:
					tmp = "StoneStep";
					tmp += num;
					break;

				case eTileSnow:
					tmp = "SnowStep";
					tmp += num;
					break;

				case eTileIce:
					tmp = "IceStep";
					tmp += num;
					break;
				default:
					break;
				}//end switch tileSpec
				SOUNDMANAGER->play(tmp, 1.f);
			}
			++m_soundeffectCount > 8 ? m_soundeffectCount = 1 : (NULL);
		}//end if aniFrameChanged

		break;	//end case Dash&Walk
	case eRolling:
	{
		//롤링 사운드 넣기
		if (nowAniIdx == 0)
		{

		}
	}
	break;
	case eShotting:
		break;
	case eStageClear:
		break;
	default: break;
	}
}

Player::Player() :
	m_moveSpeed(0.5),
	m_dashSpeed(0.8),
	m_aniChange(false),
	m_nonCansleAction(false),
	m_inGround(true),
	m_sleep(false),
	m_state(eIdle),
	m_soundeffectCount(1)
{
	m_direction = eDown;
};
Player::~Player()
{
};
