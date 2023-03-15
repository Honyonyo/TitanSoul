#include "stdafx.h"
#include "YetiProjectile.h"
#include "PixelCollision.h"

void Snowball::Init()
{
	m_imgRot = 0;
	m_img = IMAGEMANAGER->FindImage("YetiProjectile");
	m_pixelCollision = new PixelCollision(&m_center, 0, 0, m_hitboxRange, m_hitboxRange);
	m_pixelCollision->Init();
}

void Snowball::Update()
{
	if (m_move)
	{
		m_imgRot += 10;

		m_center.x += cosf(m_angle) * m_speed;
		m_center.y -= sinf(m_angle) * m_speed;

		m_attackCenter = m_center;
		m_hitboxCenter = m_center;
	}
	m_pixelCollision->Update();
	if(m_isOnHit)
	{
		if (m_pixelCollision->GetIsColl())
		{
			m_isDelete = true;
		}
	}
}

void Snowball::Render()
{
	if(m_render)
	IMAGEMANAGER->CenterFrameRender(m_img, m_center.x, m_center.y, 0, 2, eLayerUnderPlayer,1,1, m_imgRot);
}

void Snowball::Release()
{
}

void Snowball::SetRender()
{
	m_render = true;
}

void Snowball::SetMove(D2D1_POINT_2F playerPoint)
{
	m_angle = 
		MY_UTIL::getAngle
		(
			m_center.x, m_center.y, 
			playerPoint.x, playerPoint.y
		);
	m_isOnAttack = true;
	m_isOnHit = true;
	m_move = true;
}

void Snowball::Attack(eObjectKinds kinds)
{
	switch (kinds)
	{
	case ePlayer:
		SOUNDMANAGER->play("Yeti_SnowImpact", 1.f);
		break;
	case eMonsterProjectile:
		m_isDelete = true;
		break;
	}
}

void Snowball::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eMonsterProjectile:
		m_isDelete = true;
		break;
	}
}

Snowball::Snowball(float centerX, float centerY)
	:m_angle(0),m_speed(4), m_imgRot(0), m_img(nullptr), m_move(false), m_render(false),
	m_pixelCollision(nullptr)
{
	m_isDelete = false;
	m_center.x = centerX;
	m_center.y = centerY;
	m_attackCenter = m_center;
	m_hitboxCenter = m_center;
	m_attackRange = 12;
	m_hitboxRange = 12;
	m_isOnHit = true;
	m_isOnAttack = true;
	m_kinds = eMonsterProjectile;
};

void Icicle::Init()
{
	/// 고드름 떨어지는 속도 8정도..
	m_img = IMAGEMANAGER->FindImage("YetiProjectile");
	m_shadowScale = 0.01;
}

void Icicle::Update()
{
	m_lifetime -= DELTA_TIME;
	if (m_lifetime < 0)
	{
		m_isDelete = true;
	}
	if (!m_imageChange)
	{
		m_imageCenter.y += WINSIZE_Y / 2 * DELTA_TIME;
		float targetDist = m_center.y - m_imageCenter.y;
		switch (m_imgNum)
		{
		case 0:
			m_shadowScale = (targetDist>350)?0.13 : 1.3*(1-targetDist/400);
			break;
		case 1:
			m_shadowScale = (targetDist > 350) ? 0.11 : 1 * (1-targetDist / 400);
			break;
		case 2:
			m_shadowScale = (targetDist > 350) ? 0.1 : 1* (1-targetDist / 400);
			break;
		case 3:
			m_shadowScale = (targetDist > 350) ? 0.14 : 1.4 * (1-targetDist / 400);
			break;
		default:
			cout << "고드름 이미지 넘버가 이상함1. YetiProjectile.cpp" << endl;
			m_shadowScale = (targetDist > 400) ? 0.1 : 1 * (1-targetDist / 400);
			m_imgNum %= 4;
		}
		//착지직전 잠시 공격, 피격 켜기
		if (m_imageCenter.y > m_center.y - TILE_SIZE && m_imageCenter.y !=m_center.y)
		{
			m_isOnAttack = true;
			m_isOnHit = true;
		}
		//공격ON인 경우
		if (m_isOnAttack)
		{
			m_attackCenter.x = m_center.x;
			m_attackCenter.y = m_imageCenter.y + 8;
			m_attackRange = 4;

			//착지완료하면 공격은 끈다
			if (m_imageCenter.y = m_center.y)
			{
				m_isOnAttack = false;
				string soundeffectkey = "Yeti_IcicleSmash";
				int num = RND->getInt(10) + 1;
				if (num < 10)
				{
					soundeffectkey += (char)(num + 48);
				}
				else
				{
					int tmp = num / 10;
					num %= 10;
					soundeffectkey += (char)(tmp + 48);
					soundeffectkey += (char)(num + 48);
				}
				SOUNDMANAGER->play(soundeffectkey, 1.f);

			}
		}

		//목표지점에서 거리 8미만으로 남았을경우 - 착지한것으로 간주
		if (m_imageCenter.y > m_center.y - 8)
		{
			m_imageChange = true;
			m_imageCenter.y = m_center.y - 2;
			switch (m_imgNum)
			{
			case 0:
				m_shadowScale = 1.3;
				break;
			case 1:
				m_shadowScale = 1.1;
				break;
			case 2:
				m_shadowScale = 1;
				break;
			case 3:
				m_shadowScale = 1.4;
				break;
			default:
				cout << "고드름 이미지 넘버가 이상함2. YetiProjectile.cpp" << endl;
				m_shadowScale = 1;
				m_imgNum %= 4;
			}
			m_shadowScale = 1;
		}
	}
	m_shadowScale = m_shadowScale >= 1 ? 1 : m_shadowScale + 0.01;
}

void Icicle::Render()
{
	//렌더링 레이어를 만든다
	 // 배경 - 그림자 - 플레이어아래 - 플레이어위 - 벽
	 //
	 //
	eLayer layer = (m_center.y > PLAYER->GetPointF().y) ? eLayerUpperPlayer : eLayerUnderPlayer;

	if (m_imageChange)
	{
		IMAGEMANAGER->CenterFrameRender(m_img, m_center.x, m_center.y, 1, 2, eLayerShadow, 1,1, 0, 0.5);
		IMAGEMANAGER->CenterFrameRender(m_img, m_imageCenter.x, m_imageCenter.y, m_imgNum, 1, layer);
	}
	else
	{

		IMAGEMANAGER->CenterFrameRender(m_img, m_center.x+(m_img->GetFrameWidth()*(1-m_shadowScale))/2, m_center.y, 1, 2, eLayerShadow,
			m_shadowScale,m_shadowScale, 0, 0.5);
		IMAGEMANAGER->CenterFrameRender(m_img, m_imageCenter.x, m_imageCenter.y, m_imgNum, 0,layer,m_shadowScale,m_shadowScale);
	}
}

void Icicle::Release()
{
}


void Icicle::Attack(eObjectKinds kinds)
{
	switch (kinds)
	{
	case ePlayer:
		cout << "고드름 플레이어와 충돌" << endl;
		break;

	case eMonster:
	case eMonsterProjectile:
		m_isDelete = true;
		break;
	default:
		NULL;
	}
}

void Icicle::Hit(eObjectKinds kinds)
{
	switch (kinds)
	{
	case eMonster:
	case eMonsterProjectile:
		m_isDelete = true;
		break;
	default:
		NULL;
	}
}

Icicle::Icicle()
	:m_lifetime(10.0f), m_imageChange(false)
{}
Icicle::Icicle(float x, float y, int imgnum)
	:m_lifetime(10.0f), m_imageChange(false)
{
	m_isDelete = false;

	m_center.x = x;
	m_center.y = y;
	m_kinds = eMonsterProjectile;

	m_imageCenter.x = m_center.x;
	m_imageCenter.y = m_center.y - WINSIZE_Y - RND->getInt(50);

	m_isOnHit = false;
	m_isOnAttack = false;

	m_imgNum = imgnum;
}
Icicle::~Icicle()
{}

void Icicle::Setting(float x, float y, int imgnum)
{
	m_center.x = x;
	m_center.y = y;

	m_imageCenter.x = m_center.x;
	m_imageCenter.y = m_center.y - WINSIZE_Y - RND->getInt(100);

	string soundeffectkey = "Yeti_IcicleLand";
	int num = RND->getInt(12) + 1;
	if (num < 10)
	{
		soundeffectkey += (char)(num + 48);
	}
	else
	{
		int tmp = num / 10;
		num %= 10;
		soundeffectkey += (char)(tmp + 48);
		soundeffectkey += (char)(num + 48);
	}
	SOUNDMANAGER->play(soundeffectkey, 1.f);

	m_imgNum = imgnum;
};

