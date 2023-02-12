#include "stdafx.h"
#include "Scene.h"

void Scene::LoadTile(vector<tagTile*>* tileLayerVector, string filePath)
{
	Json::Value root = JSONDATAMANAGER->loadJsonFile(filePath.c_str());	//json���� �ε�

	//�� �ϳ��� �������� Tile�� ��
	//�ش� �ʿ��� ���Ǵ� Ÿ�Ͻ�Ʈ�� �������ֱ�
	for (auto tileSheetIt = root["tileSheet"].begin(); tileSheetIt != root["tileSheet"].end(); tileSheetIt++)
	{
		AddTileSheet((*tileSheetIt)["key"].asString(), (*tileSheetIt)["firstGrid"].asInt());	//����ϴ� Ÿ�Ͻ�Ʈ ���
		string origin = (*tileSheetIt)["key"].asString();
	}
	//Json�� ���ǵ� Tile�迭�� �� �ֱ� ���� �ݺ���
	for (auto it = root["Tile"].begin(); it != root["Tile"].end(); it++)
	{
		tagTile* tileSet = new tagTile;	//tileV�� push_back�ϱ� ���� �غ��� �ӽ� ����
		tileSet->layerType = IMAGEMANAGER->LayerTypeStringToEnum((*it)["layerType"].asString());	//���̾� Ÿ�� �ֱ�(enum���� ��ȯ�Ͽ�)
		tileSet->number = (*it)["layerNumber"].asInt();
		//json�� ���ǵ� Ÿ�� �� ����ĭ��, �� ����ĭ�� �Է� - for�� ������ ������
		int width = (*it)["tileWidth"].asInt();
		int height = (*it)["tileHeight"].asInt();
		auto tileNumberIt = (*it)["tileCode"].begin();
		for (int i = 0; i < height && tileNumberIt != (*it)["tileCode"].end(); i++)
		{
			vector<int> low;
			for (int j = 0; j < width && tileNumberIt != (*it)["tileCode"].end(); j++)
			{
				low.push_back((*tileNumberIt).asInt());
				tileNumberIt++;
			}//end for width
			tileSet->tile.push_back(low);
			low.clear();
			vector<int>().swap(low);
		}//end for heigth
		tileSet->layerImage = IMAGEMANAGER->AddTileLayerImage(mapTileSheetList, tileSet->tile);
		(m_tileLayerVector).push_back(tileSet);

		cout << "Ÿ�� ���� ����" << endl;
	}

	for (int i = 0; i < m_tileLayerVector.size(); i++)
	{
		if (m_tileLayerVector[i]->layerType == eCOL)
		{
			m_collLayer = new GImage;
			m_collLayer->Init(m_tileLayerVector[i]->tile[0].size()*TILE_SIZE, m_tileLayerVector[i]->tile.size() * TILE_SIZE);

			int tileFirstNum = NULL;
			for (tileSheetList::iterator iter = mapTileSheetList.begin(); iter != mapTileSheetList.end(); iter++)
			{
				if (iter->second == tile_spectiles)
				{
					tileFirstNum = iter->first;
					break;
				}

			}
			if (tileFirstNum == NULL)
			{
				cout << "GImage�� �浹�̹��� ����°� ���� �߸���. (Scene.cpp LoadTile func)" << endl;
			}
			m_collLayer->TileRender(IMAGEMANAGER->FindSpecTile(), m_tileLayerVector[i]->tile, tileFirstNum);
			break;
		}
	}

	m_mapsizeX = m_tileLayerVector[0]->layerImage->GetFrameWidth();
	m_mapsizeY = m_tileLayerVector[0]->layerImage->GetFrameHeight();

	isLoaded = true;
};
