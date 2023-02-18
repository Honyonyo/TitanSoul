#pragma once
#include "GameNode.h"

class Scene : public GameNode
{
protected:

	typedef vector<vector<int>> tileVector;
	typedef map<int, eTileSheetKey> tileSheetList;

	struct tagTile
	{
		tileVector tile;
		eTileLayerType layerType;
		int number;
		CImage* layerImage;

		tagTile() :
			number(0), layerImage(nullptr),
			layerType(eTileLayerType::eTileLayerTypeCount)
		{};
		~tagTile()
		{
			SAFE_DELETE(layerImage);
		}
	};

protected:
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

public:
	virtual HRESULT Init(void) PURE;
	virtual void Release(void) PURE;
	virtual void Update(void) PURE;
	virtual void Render(void) PURE;

	bool GetIsLoaded(void) { return isLoaded; }

	void LoadTile(vector<tagTile*>* tileLayerVector, string filePath);
	void AddTileSheet(string key, int firstTileNumber)
	{
		eTileSheetKey enumKey = IMAGEMANAGER->TileKeyStringToEnum(key);
		mapTileSheetList.insert(make_pair(firstTileNumber, enumKey));
	}

	virtual D2D1_POINT_2F GetPlayerStartPos(void) { return m_playerStartPos; }
	virtual int GetMattileSpec(int latitude, int longitude) { return  (*m_matTileLayer)->tile[latitude][longitude]; }
	virtual COLORREF** GetPixelList(void) { return m_collLayer->_pixel; }
	virtual float GetMapWidth()
	{
		if (this->m_tileLayerVector.empty()==true)
		{
			cout << "맵에 타일 아직 안들어감. 사이즈 호출 실패" << endl;
			return NULL;
		}
		return this->m_mapsizeX;
	}
	virtual float GetMapHeight()
	{
		if (this->m_tileLayerVector.empty() == true)
		{
			cout << "맵에 타일 아직 안들어감. 사이즈 호출 실패" << endl;
			return NULL;
		}
		return this->m_mapsizeY;
	};

	Scene() 
		: isLoaded(false), m_mapsizeX(NULL), m_mapsizeY(NULL) 
	{}
	virtual ~Scene() 
	{
		if (m_collLayer != nullptr)
		{
			m_collLayer->Release();
			SAFE_DELETE(m_collLayer);
		}
	}
};