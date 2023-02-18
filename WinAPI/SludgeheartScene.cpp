#include "stdafx.h"
#include "SludgeheartScene.h"

HRESULT SludgeheartScene::Init(void)
{
	if (isLoaded == false)
	{
		LoadTile(&m_tileLayerVector, "SlugeheartMap.json");

		isLoaded = true;
	}
	m_sludgeheart = new Sludgeheart;
	m_sludgeheart->Init();

	OBJECTMANAGER->AddObject(m_sludgeheart);

	return S_OK;
}

void SludgeheartScene::Release(void)
{
	if (m_sludgeheart != nullptr)
	{
		m_sludgeheart->Release();
	}
}

void SludgeheartScene::Update(void)
{
}

void SludgeheartScene::Render(void)
{
	m_iterTileLayerVector = m_tileLayerVector.begin();

	for (; m_iterTileLayerVector != m_tileLayerVector.end(); m_iterTileLayerVector++)
	{
		switch ((*m_iterTileLayerVector)->layerType)
		{
		case eBG:	//¹Ù´Ú ·»´õ
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2, m_mapsizeY / 2, 0, 0, eLayerBg);
			break;
		case eFG:	//º® ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2, m_mapsizeY / 2, 0, 0, eLayerWall);
			}
			break;
		case eCOL:	//Ãæµ¹ ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F2))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2, m_mapsizeY / 2, 0, 0, eLayerUnderPlayer);
			}
			break;
		case eMAT:	//±â´É¹ßÆÇ ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F3))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2, m_mapsizeY / 2, 0, 0, eLayerUnderPlayer);
			}
			break;
		}
	}
}

SludgeheartScene::SludgeheartScene()
	:m_sludgeheart(nullptr)
{
	LoadTile(&m_tileLayerVector, "SlugeheartMap.json");
	m_playerStartPos = { 496.0f, 912.0f };
}

SludgeheartScene::~SludgeheartScene()
{
}
