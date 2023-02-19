#include "stdafx.h"
#include "YetiScene.h"

HRESULT YetiScene::Init(void)
{
	//º¸½ºÆÐÅÏ Å¥¿¡ ³Ö°í µ¹¸®±â (¼ÅÇÃ) °³Â¾´ç
	if (isLoaded == false)
	{
		LoadTile(&m_tileLayerVector, "YetiMap.json");

		isLoaded = true;
	}

	m_yeti = new Yeti;
	m_yeti->Init();

	OBJECTMANAGER->AddObject(m_yeti);

	return S_OK;
}

void YetiScene::Release(void)
{
	if (m_yeti != nullptr)
	{
		m_yeti->Release();
		SAFE_DELETE(m_yeti);
	}
}

void YetiScene::Update(void)
{
}

void YetiScene::Render(void)
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
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2, m_mapsizeY / 2, 0, 0, eLayerWall);
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
		if (KEYMANAGER->isToggleKey(VK_F4))
		{
			IMAGEMANAGER->CenterFrameRender(m_collLayer->GetRenderImage(), m_mapsizeX / 2, m_mapsizeY / 2, 0, 0, eLayerUnderPlayer);

		}
	}
}

YetiScene::YetiScene()
	:m_yeti(nullptr)
{
	LoadTile(&m_tileLayerVector, "YetiMap.json");
	m_playerStartPos = { 496.0f, 912.0f };
};
YetiScene::~YetiScene()
{
};
