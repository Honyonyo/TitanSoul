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
		//�����̰� 0�϶� : ���İ� ����.
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

		//�����̰� 1 �̸��� �� : ��� ��µǰ��ִ� ��.
		else if (m_delay < delayTime)
		{
			if (m_alpha == 1)
			{
				m_delay += DELTA_TIME;
				if (m_delay > delayTime)	//�����̰� 1�� �Ѿ�� ���İ��� ���ҽ�Ų��. ������ ���İ� 0�̸��� �Ǹ� ����, ������ �ʱ�ȭ. num+1;
				{
					m_delay = delayTime;

				}
			}
		}

		//�����̰� �Ѿ�� ���İ��� ���ҽ�Ų��. ������ ���İ� 0�̸��� �Ǹ� ����, ������ �ʱ�ȭ. num+1;
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
	//imageNumber�� 3�Ѿ�� �޴�����ȭ�� ����
	/*
	���İ� ����->���İ� 1�̵Ǹ� ������ ����->�����̰� 1�� �Ǹ� ���� ����->���� 0�Ǹ� ���� �̹����� ��ȯ �� �ݺ�
	*/
}

void IntroScene::Render(void)
{
	m_iterTileLayerVector = m_tileLayerVector.begin();

	for (; m_iterTileLayerVector != m_tileLayerVector.end(); m_iterTileLayerVector++)
	{
		switch ((*m_iterTileLayerVector)->layerType)
		{
		case eBG:	//�ٴ� ����
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerBg);
			break;
		case eWall:
		case eFG:	//�� ����
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerWall);
			}
			break;
		case eCOL:	//�浹 ����
			if (KEYMANAGER->isToggleKey(VK_F2))
			{
				IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX / 2 + TILE_SIZE, m_mapsizeY / 2 + TILE_SIZE, 0, 0, eLayerUnderPlayer);
			}
			break;
		case eMAT:	//��ɹ��� ����
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
