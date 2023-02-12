#include "stdafx.h"
#include "YetiScene.h"

HRESULT YetiScene::Init(void)
{
	//�������� ť�� �ְ� ������ (����) ��¾��
	if (isLoaded == false)
	{
		LoadTile(&m_tileLayerVector, "YetiMap.json");

		isLoaded = true;
	}

	PLAYER->SetPosition(496.0f, 912.0f);
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
	IMAGEMANAGER->DrawRect({ 0,0,WINSIZE_X, WINSIZE_Y });
	m_iterTileLayerVector = m_tileLayerVector.begin();

	for (; m_iterTileLayerVector != m_tileLayerVector.end(); m_iterTileLayerVector++)
	{
		switch ((*m_iterTileLayerVector)->layerType)
		{
		case eBG:	//�ٴ� ����
			IMAGEMANAGER->CenterFrameRender((*m_iterTileLayerVector)->layerImage, m_mapsizeX/2, m_mapsizeY/2, 0, 0,eLayerBg);
			break;
		case eFG:	//�� ����
			if (KEYMANAGER->isToggleKey(VK_F1))
			{
				IMAGEMANAGER->Render((*m_iterTileLayerVector)->layerImage, 0, 0);
			}
			break;
		case eCOL:	//�浹 ����
			if (KEYMANAGER->isToggleKey(VK_F2))
			{
				IMAGEMANAGER->Render((*m_iterTileLayerVector)->layerImage, 0, 0);
			}
			break;
		case eMAT:	//��ɹ��� ����
			if (KEYMANAGER->isToggleKey(VK_F3))
			{
				IMAGEMANAGER->Render((*m_iterTileLayerVector)->layerImage, 0, 0);
			}
			break;
		}
	}

}

YetiScene::YetiScene()
{
	LoadTile(&m_tileLayerVector, "YetiMap.json");

};
YetiScene::~YetiScene()
{

};
