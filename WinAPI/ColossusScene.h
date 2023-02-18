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
	bool isLoaded;	//생성자에서 false로 초기화, Init되면 true로 변경
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

