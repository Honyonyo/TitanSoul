#include "stdafx.h"
#include "YetiProjectile.h"

void Icicle::Init()
{
	/// 고드름 떨어지는 속도 8정도..
	/// 
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
		if (m_imageCenter.y > m_center.y - 8)
		{
			m_imageChange = true;
			m_imageCenter.y = m_center.y - 2;
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


	if (m_imageChange)
	{
		IMAGEMANAGER->CenterFrameRender(m_img, m_center.x, m_center.y, 1, 2, 1, 0, 0.5);
		IMAGEMANAGER->CenterFrameRender(m_img, m_imageCenter.x, m_imageCenter.y, m_imgNum, 1, 1);
	}
	else
	{

		IMAGEMANAGER->CenterFrameRender(m_img, m_center.x, m_center.y, 1, 2,
			m_shadowScale, 0, 0.5);
		IMAGEMANAGER->CenterFrameRender(m_img, m_imageCenter.x, m_imageCenter.y, m_imgNum, 0, 1);
	}
}

void Icicle::Release()
{
}


void Icicle::Attack(eObjectKinds kinds)
{
}

void Icicle::Hit(eObjectKinds kinds)
{
}

Icicle::Icicle()
	:m_lifetime(10.0f), m_imageChange(false)
{}
Icicle::Icicle(float x, float y, int imgnum)
	:m_lifetime(10.0f), m_imageChange(false)
{
	m_center.x = x;
	m_center.y = y;

	m_imageCenter.x = m_center.x;
	m_imageCenter.y = m_center.y - WINSIZE_Y - RND->getInt(50);

	m_imgNum = imgnum;
}

void Icicle::Setting(float x, float y, int imgnum)
{
	m_center.x = x;
	m_center.y = y;

	m_imageCenter.x = m_center.x;
	m_imageCenter.y = m_center.y - WINSIZE_Y - RND->getInt(50);

	m_imgNum = imgnum;
};
