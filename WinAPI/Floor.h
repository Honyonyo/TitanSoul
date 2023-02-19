#pragma once
#include "Scene.h"
class Colossus;
class Floor : public Scene
{
private:

	//typedef vector<vector<int>> tileVector;
	//typedef map<int, eTileSheetKey> tileSheetList;

	//struct tagTile
	//{
	//	tileVector tile;
	//	eTileLayerType layerType;
	//	int number;
	//	CImage* layerImage;

	//	tagTile() :
	//		number(0), layerImage(nullptr),
	//		layerType(eTileLayerType::eTileLayerTypeCount)
	//	{};
	//	~tagTile()
	//	{
	//		SAFE_DELETE(layerImage);
	//	}
	//};
	//tileSheetList mapTileSheetList;
	//vector<tagTile*> m_tileLayerVector;
	//vector<tagTile*>::iterator m_iterTileLayerVector;
	//GImage* m_collLayer = nullptr;
	//float m_mapsizeX;
	//float m_mapsizeY;

	//bool isLoaded;	//생성자에서 false로 초기화, Init되면 true로 변경
	RECT m_yetiGate;
	Colossus* m_colossus;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	Floor();
	~Floor();
};

