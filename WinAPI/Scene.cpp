#include "stdafx.h"
#include "Scene.h"

void Scene::LoadTile(vector<tagTile*>* tileLayerVector, string filePath)
{
	Json::Value root = JSONDATAMANAGER->loadJsonFile(filePath.c_str());	//json파일 로드

	//맵 하나에 여러개의 Tile이 들어감
	//해당 맵에서 사용되는 타일시트를 저장해주기
	for (auto tileSheetIt = root["tileSheet"].begin(); tileSheetIt != root["tileSheet"].end(); tileSheetIt++)
	{
		AddTileSheet((*tileSheetIt)["key"].asString(), (*tileSheetIt)["firstGrid"].asInt());	//사용하는 타일시트 등록
		string origin = (*tileSheetIt)["key"].asString();
	}
	//Json에 정의된 Tile배열을 다 넣기 위한 반복문
	for (auto it = root["Tile"].begin(); it != root["Tile"].end(); it++)
	{
		tagTile* tileSet = new tagTile;	//tileV에 push_back하기 위해 준비한 임시 변수
		tileSet->layerType = IMAGEMANAGER->LayerTypeStringToEnum((*it)["layerType"].asString());	//레이어 타입 넣기(enum으로 변환하여)
		tileSet->number = (*it)["layerNumber"].asInt();
		//json에 정의된 타일 총 가로칸수, 총 세로칸수 입력 - for문 돌릴때 용이함
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

		cout << "타일 정보 넣음" << endl;
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
				cout << "GImage로 충돌이미지 만드는거 뭔가 잘못됨. (Scene.cpp LoadTile func)" << endl;
			}
			m_collLayer->TileRender(IMAGEMANAGER->FindSpecTile(), m_tileLayerVector[i]->tile, tileFirstNum);
			break;
		}
	}

	m_mapsizeX = m_tileLayerVector[0]->layerImage->GetFrameWidth();
	m_mapsizeY = m_tileLayerVector[0]->layerImage->GetFrameHeight();

	isLoaded = true;
};
