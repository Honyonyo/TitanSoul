#include "stdafx.h"
#include "Colossus.h"

void Colossus::Init()
{
	//Image Setting
	m_image[eBody] = IMAGEMANAGER->FindImage("ColossusBody");
	m_image[eBodyLight] = IMAGEMANAGER->FindImage("ColossusBodyLight");
	m_image[eHand] = IMAGEMANAGER->FindImage("ColossusHand");
	m_image[eHandShadow] = IMAGEMANAGER->FindImage("ColossusHandShadow");
	m_image[eHandSleep] = IMAGEMANAGER->FindImage("ColossusHandSleep");
	m_image[eHead] = IMAGEMANAGER->FindImage("ColossusHead");
}

void Colossus::Update()
{

}

void Colossus::Render()
{
	IMAGEMANAGER->CenterFrameRender(m_image[eBody], m_center.x, m_center.y, 0, 0, eLayerUnderPlayer);
	IMAGEMANAGER->CenterFrameRender(m_image[eHead], m_center.x, m_center.y - 48	, 1, 0, eLayerUnderPlayer);
	IMAGEMANAGER->CenterFrameRender(m_image[eHandSleep], m_center.x, m_center.y + 48, 0, 0, eLayerUnderPlayer);
}

void Colossus::Release()
{
	m_isDelete = true;
}

void Colossus::Attack(eObjectKinds kinds)
{

}

void Colossus::Hit(eObjectKinds kinds)
{
}

Colossus::Colossus()
{
	m_center = { 800 + 8, 896 - TILE_SIZE };
	m_wakeup = false;
}

Colossus::~Colossus()
{
}
