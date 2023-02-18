#include "stdafx.h"
#include "IntroScene.h"

HRESULT IntroScene::Init(void)
{
	if (isLoaded == false)
	{
		LoadTile(&m_tileLayerVector, "Title.json");

		isLoaded = true;
	}

	m_UIimage[0] = IMAGEMANAGER->FindImage("UI_GamePad");
	m_UIimage[1] = IMAGEMANAGER->FindImage("UI_AN");
	m_UIimage[2] = IMAGEMANAGER->FindImage("UI_Devolver");

	return S_OK;
}

void IntroScene::Release(void)
{
}

void IntroScene::Update(void)
{
	if (m_imageNumber < 3)
	{
		float delayTime = 2;
		//딜레이가 0일때 : 알파값 증가.
		if (m_delay == 0)
		{
			if (m_alpha < 1)
				m_alpha += 0.01;
			if (m_alpha > 1)
			{
				m_alpha = 1;
				m_delay += DELTA_TIME;
			}
		}

		//딜레이가 1 미만일 때 : 계속 출력되고있는 중.
		else if (m_delay < delayTime)
		{
			if (m_alpha == 1)
			{
				m_delay += DELTA_TIME;
				if (m_delay > delayTime)	//딜레이가 1이 넘어가면 알파값을 감소시킨다. 감소한 알파가 0미만이 되면 알파, 딜레이 초기화. num+1;
				{
					m_delay = delayTime;

				}
			}
		}

		//딜레이가 넘어가면 알파값을 감소시킨다. 감소한 알파가 0미만이 되면 알파, 딜레이 초기화. num+1;
		if (m_delay == delayTime)
		{
			m_alpha -= 0.05;
			if (m_alpha < 0)
			{
				m_alpha = 0;
				m_delay = 0;
				m_imageNumber++;
			}
		}
	}
	else if (m_imageNumber == 3)
	{
		if (m_alpha < 1)
			m_alpha += 0.01;
		if (m_alpha > 1)
		{
			m_alpha = 1;
		}
		if(m_alpha ==1 )
		{
			m_imageNumber++;
		}
	}

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN))
	{
		if (m_imageNumber < 3) m_imageNumber = 3;
		else
		{
			SceneChange();
		}
	}
	//imageNumber가 3넘어가면 메뉴선택화면 띄우기
	/*
	알파값 증가->알파가 1이되면 딜레이 증가->딜레이가 1이 되면 알파 감소->알파 0되면 다음 이미지로 전환 후 반복
	*/
}

void IntroScene::Render(void)
{
	m_iterTileLayerVector = m_tileLayerVector.begin();

	for (; m_iterTileLayerVector != m_tileLayerVector.end(); m_iterTileLayerVector++)
	{
		switch ((*m_iterTileLayerVector)->layerType)
		{
		case eBG:	//바닥 렌더
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerBg);
			break;
		case eWall:
		case eFG:	//벽 렌더
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerWall);
			}
			break;
		case eCOL:	//충돌 렌더
			if (KEYMANAGER->isToggleKey(VK_F2))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		case eMAT:	//기능발판 렌더
			if (KEYMANAGER->isToggleKey(VK_F3))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		}
	}

	if (m_imageNumber < 3)
	{
		IMAGEMANAGER->DrawBlackRect({ 0,0,WINSIZE_X,WINSIZE_Y }, eLayerTop);
		IMAGEMANAGER->CenterUIRender(m_UIimage[m_imageNumber], WINSIZE_X / 2, WINSIZE_Y / 2,1,1,0,false,m_alpha);
	}
	if (m_imageNumber == 3)
	{
		IMAGEMANAGER->DrawBlackRect({ 0,0,WINSIZE_X,WINSIZE_Y }, eLayerTop, 1-m_alpha);
	}
}

void IntroScene::SceneChange(void)
{
	PLAYER->SetSleepOnOff(false);
	SCENEMANAGER->changeScene("Floor");
}

IntroScene::IntroScene()
	:m_alpha(0), m_delay(0.f), m_imageNumber(0)
{
	LoadTile(&m_tileLayerVector, "Title.json");
	m_playerStartPos = { 568, 568 };
	PLAYER->SetSleepOnOff(true);
}

IntroScene::~IntroScene()
{
}
