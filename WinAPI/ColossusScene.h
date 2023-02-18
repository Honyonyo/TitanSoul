#pragma once
#include "Scene.h"
class Colossus;
class ColossusScene : public Scene
{
private:
	/*
		tileSheetList mapTileSheetList;
	vector<tagTile*> m_tileLayerVector;
	vector<tagTile*>::iterator m_iterTileLayerVector;
	vector<tagTile*>::iterator m_matTileLayer;
	GImage* m_collLayer = nullptr;
	float m_mapsizeX;
	float m_mapsizeY;

	D2D1_POINT_2F m_playerStartPos;
	bool isLoaded;	//�����ڿ��� false�� �ʱ�ȭ, Init�Ǹ� true�� ����
	int m_specLayerNumber;
	*/
	Colossus* m_colossus;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	ColossusScene();
	~ColossusScene();
};

