#include "stdafx.h"
#include "Floor.h"
#include "Colossus.h"

HRESULT Floor::Init(void)
{
	if (isLoaded == false)
	{
		LoadTile(&m_tileLayerVector, "Floor.json");

		isLoaded = true;
	}

	m_colossus = new Colossus;
	m_colossus->Init();
	m_colossus->SetPosition(800.f + 8, 896.f - TILE_SIZE);
	OBJECTMANAGER->AddObject(m_colossus);
	m_yetiGate=RectMakeCenter(800, 1584, 32, 64);

    return S_OK;
}

void Floor::Release(void)
{
}

void Floor::Update(void)
{
	if (PtInRect(&m_yetiGate, PLAYER->GetCenterPOINT()))
	{
		SCENEMANAGER->changeScene("YetiScene");
	}
}

void Floor::Render(void)
{
	m_iterTileLayerVector = m_tileLayerVector.begin();

	for (; m_iterTileLayerVector != m_tileLayerVector.end(); m_iterTileLayerVector++)
	{
		switch ((*m_iterTileLayerVector)->layerType)
		{
		case eBG:	//¹Ù´Ú ·»´õ
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2+ TILE_SIZE, m_mapsizeY / 2+ TILE_SIZE, 0, 0, eLayerBg);
			break;
		case eWall:
			//IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerWall);
			//break;
		case eFG:	//º® ·»´õ
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2+ TILE_SIZE, m_mapsizeY / 2+ TILE_SIZE, 0, 0, eLayerWall);
			break;
		case eCOL:	//Ãæµ¹ ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F2))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2+ TILE_SIZE, m_mapsizeY / 2+ TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		case eMAT:	//±â´É¹ßÆÇ ·»´õ
			if (KEYMANAGER->isToggleKey(VK_F3))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2+ TILE_SIZE, m_mapsizeY / 2+ TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		}
	}
}

Floor::Floor()
{
	LoadTile(&m_tileLayerVector, "Floor.json");

	PLAYER->SetSleepOnOff(false);
	m_playerStartPos = { 808.0f, 3288.0f };
	//m_playerStartPos = { 800.0f, 1000.0f };
}

Floor::~Floor()
{
}
