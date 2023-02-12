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
	m_direction = eUp;
	m_state = eIdle;
	//������ �÷��̾� ��ġ �ʱⰪ�� ����ָ� �װŷ� �����ϱ�
	//Ŀ��Ʈ���� �ؽ�Ʈ���� ���ؼ� �ؽ�Ʈ���� switch�� �÷��̾���ġ�ʱⰪ ���Ǵ޾Ƽ� ����ֱ�
	m_animation->SetPlayFrame(m_aniIndexArr[1][m_direction][m_state],true);

	m_center.x = 496.0f;
	m_center.y = 912.0f;

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
	m_aniChange = false;

	/// XŰ�� ������ �����⵿������ ��쿡�� ������� ��ĵ���̶� ������ȯ�� �ȵǰ�, state���浵 �ȵȴ�.
	/// <state���濡���Ͽ�>
	/// ���۽� state�� prevState�� ������ �� �⺻�� idle�� �ٲٰ�
	/// Update���� ���� Ŀ��尡 ���� ��� state�� �ش� Ŀ���� �ٲ۴�.
	/// prevState�� state�� ������ ���� ������ �̾ �ϴ°��̹Ƿ� direction�� �ٲ� �� ������ �����ϰ� �ִϸ��̼� ���,
	/// �ƴѰ�� ������ �ƿ� �ٲ���̹Ƿ� direction�� �ش��ϴ� �� state�ִϸ��̼��� 0��° �����Ӻ��� ����Ѵ�.
	/// </state���濡���Ͽ�>
	/// <direction���濡���Ͽ�>
	/// ȭ��ǥŰ�� ������ ������ ���ϴµ�,
	/// CŰ�� �ȴ����������� �Ȱų� �ٸ鼭 ������ ��ȯ�ȴ�.
	/// arrow�� ���� ��� CŰ�� ������������ �� ���� ������ �ٲٴ� ���� ����, ������ ���������ε� ������ȯ �Ұ��ϴ�.
	/// </direction���濡���Ͽ�>

	int frameIndex = 0;
	eMoveDirection prevDirction = m_direction;
	eState prevState = m_state;
	m_state = eIdle;

	//CŰ ������ ī�޶� Ȯ��Ǳ�
	//ȭ�� ������ - �߻��� / ȭ�� ������ - ȸ�����
	if (KEYMANAGER->isStayKeyDown('C'))
	{
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

	}//end if 'C'		�������� - �̵��ϴ� ��ǥ���� ���� ������ ��� ����(�� �ʿ� ������?)
	else//C�ȴ����ų� ��������� �ƴ� ��쿡�� MOVE����
	{
		Move();
	}//end if not 'C'

	//CŰ ���� ī�޶� �ٽ� ���ƿ���
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
	}

	if (m_direction != prevDirction
		|| m_state != prevState)
	{
		m_aniChange = true;
		if (m_state == prevState)
			frameIndex = m_animation->GetNowFrameIdx();
	}

	if (m_aniChange)
	{
		m_animation->SetPlayFrame(m_aniIndexArr[1][m_direction][m_state], true, frameIndex);
	}

	m_hitboxCenter = m_center;
	m_animation->FrameUpdate(TIMEMANAGER->getElapsedTime());

	m_pixelCollision->Update();
};
void Player::Render()
{
	//�׸��� ����
	//IMAGEMANAGER->CenterFrameRender(m_Image, );

	IMAGEMANAGER->CenterAniRender(m_Image, m_center.x, m_center.y, m_animation, eLayerTop);
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
		break;
	default:
		NULL;
	}
};

void Player::Move()
{
	float speed = (KEYMANAGER->isStayKeyDown('X')) ? m_dashSpeed : m_moveSpeed;
	bool isDash = (speed == m_dashSpeed) ? true : false;

	eMoveDirection prevDirection = m_direction;

	m_directionKey.reset();

	/// <summary>
	/// ȭ��ǥ Ű�� ������ �� : ������ȯ
	/// ���� �� : �ٸ� Ű�� �������ִٸ� : ������ȯ ->��� �Ǻ�? state�� idle�� �ƴ� ���� �Ǻ��ϱ�
	/// ���� �� : �ٸ� Ű�� �����°� ���ٸ� : Ű ���� ���� ������ �ٶ󺸰� ���߱� ->state�� idle�� ���
	/// </summary>

	if (KEYMANAGER->isStayKeyDown(VK_LEFT))
	{
		if (!m_pixelCollision->GetLeftColl()) m_center.x -= speed;
		m_directionKey[0] = 1;
		m_directionKey[2] = 0;
		m_state = isDash ? eDash : eWalk;
	}
	if (KEYMANAGER->isStayKeyDown(VK_UP))
	{
		if (!m_pixelCollision->GetTopColl())	m_center.y -= speed;
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
	}
	if (KEYMANAGER->isStayKeyDown(VK_DOWN))
	{
		if (!m_pixelCollision->GetBottomColl()) m_center.y += speed;
		m_directionKey[1] = 0;
		m_directionKey[3] = 1;
		m_state = isDash ? eDash : eWalk;
	}
	if (m_state != eIdle)
	{
		int direction = m_directionKey.to_ulong();

		//bitset�� ��Ʋ��������� ���°�����..........  �Ф�
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

};

void Player::SetAnimationFrame()
{
	for (int i = 0; i < eMoveDirectionNumber; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			m_aniIndexArr[1][i][eWalk].push_back(j + m_animation->GetFrameNumWidth() * i);
			m_aniIndexArr[1][i][eRolling].push_back(i * (6 + m_animation->GetFrameNumWidth()) + j);
		}
		for (int j = 0; j < 12; j++)
		{
			m_aniIndexArr[1][i][eDash].push_back(j + ((8 + i) * m_animation->GetFrameNumWidth()) + 8);
		}
		m_aniIndexArr[1][i][eShotting].push_back(13 + m_animation->GetFrameNumWidth() * i);
		m_aniIndexArr[1][i][eCall].push_back(14 + m_animation->GetFrameNumWidth() * i);
		m_aniIndexArr[1][i][eIdle].push_back(i * m_animation->GetFrameNumWidth());
		//m_aniIndexArr[1][i][eRollingSuccess].push_back(i * 72);
		//m_aniIndexArr[1][i][eRollingFail].push_back(i * 71);
		//for (int j = 0; j < 6; j++)
		//{
		//	
		//}
		//for (int j = 0; j < 12; j++)
		//{
		//	m_aniIndexArr[1][i][eDash].push_back(i * (64) + 8 + j + 64 * 8);
		//}
	}

	for (int i = 0; i < m_aniIndexArr[1][eRight][eWalk].size(); i++)
		m_animation->SetFPS(3);
	//m_animation->SetPlayFrame(m_aniIndexArr[1][eUp][eIdle]);
	m_animation->AniStart();
}

Player::Player() :
	m_moveSpeed(0.5),
	m_dashSpeed(1),
	m_aniChange(false),
	m_inGround(true),
	m_state(eIdle)
{
	m_direction = eDown;
};
Player::~Player()
{
};
