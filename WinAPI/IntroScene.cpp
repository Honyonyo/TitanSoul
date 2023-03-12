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
	}//end if imageNum < 3
	else if (m_imageNumber == 3)
	{
		if (m_alpha < 0.6)
			m_alpha += 0.005;
		if (m_alpha > 0.6f)
		{
			m_alpha = 0.6f;
		}
		if (m_alpha == 0.6f)
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
		case eWall:
		case eFG:	//�� ����
			IMAGEMANAGER->CenterUIRender((*m_iterTileLayerVector)->layerImage, WINSIZE_X / 2, WINSIZE_Y / 2 + TILE_SIZE, 0, 0);
			break;
		default:
			NULL;
		}
	}

	if (m_imageNumber < 3)
	{
		IMAGEMANAGER->DrawBlackRect({ 0,0,WINSIZE_X,WINSIZE_Y }, eLayerTop);
		IMAGEMANAGER->CenterUIRender(m_UIimage[m_imageNumber], WINSIZE_X / 2, WINSIZE_Y / 2, 0, 0, 1, 1, 0, false, m_alpha);
	}
	if (m_imageNumber >= 3)
	{
		IMAGEMANAGER->DrawBlackRect({ 0,0,WINSIZE_X,WINSIZE_Y }, eLayerTop, 1 - m_alpha);
	}
	if (m_imageNumber == 4)
	{
		IMAGEMANAGER->CenterUIRender(IMAGEMANAGER->FindImage("UI_Title"), WINSIZE_X / 2, WINSIZE_Y / 4, 0, 0, 2, 2, 0, false, m_alpha);
		char text[5] = { 'S','T','A','R','T' };
		for (int i = 0; i < 5; i++)
		{
			int frameY = text[i] / 32;
			int frameX = text[i] - frameY * 32;
			IMAGEMANAGER->CenterUIRender(IMAGEMANAGER->FindImage("UI_Text"), WINSIZE_X / 2 + (i - 2.0f) * TILE_SIZE, WINSIZE_Y / 2 + 100, frameX, frameY, 1, 1, 0);
		}
		IMAGEMANAGER->CenterUIRender(IMAGEMANAGER->FindImage("UI_MenuCursur"), WINSIZE_X / 2 - 4.0f * TILE_SIZE, WINSIZE_Y / 2 + 100, 3, 0, 1, 1, 0);
		IMAGEMANAGER->CenterUIRender(IMAGEMANAGER->FindImage("UI_MenuCursur"), WINSIZE_X / 2 + 3.5f * TILE_SIZE, WINSIZE_Y / 2 + 100, 2, 0, 1, 1, 0);

	}
}

void IntroScene::SetOn()
{
	SOUNDMANAGER->play("BGMIntro", 1.f);
}
void IntroScene::SetOff()
{
	SOUNDMANAGER->stop("BGMIntro");
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
