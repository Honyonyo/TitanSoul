#include "stdafx.h"
#include "ColossusScene.h"
#include "Colossus.h"

HRESULT ColossusScene::Init(void)
{
	if (isLoaded == false)
	{
		LoadTile(&m_tileLayerVector, "ColossusMap.json");

		isLoaded = true;
	}

	m_colossus = new Colossus;
	m_colossus->Init();
	m_colossus->SetPosition(480.f, 288.f);
	OBJECTMANAGER->AddObject(m_colossus);

	return S_OK;
}

void ColossusScene::Release(void)
{
	m_colossus->Release();
}

void ColossusScene::Update(void)
{
}

void ColossusScene::Render(void)
{
	m_iterTileLayerVector = m_tileLayerVector.begin();

	for (; m_iterTileLayerVector != m_tileLayerVector.end(); m_iterTileLayerVector++)
	{
		switch ((*m_iterTileLayerVector)->layerType)
		{
		case eBG:	//¹Ù´Ú ·»´õ
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerBg);
			break;
		case eWall:
		case eFG:	//º® ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerWall);
			}
			break;
		case eCOL:	//Ãæµ¹ ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F2))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		case eMAT:	//±â´É¹ßÆÇ ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F3))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		}
	}
}

ColossusScene::ColossusScene()
:m_colossus(nullptr)
{
	LoadTile(&m_tileLayerVector, "ColossusMap.json");
	m_playerStartPos = { 480.f, 416.f };
}

ColossusScene::~ColossusScene()
{
}
