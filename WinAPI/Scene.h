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
	GImage* m_collLayer = nullptr;

	bool isLoaded;	//�����ڿ��� false�� �ʱ�ȭ, Init�Ǹ� true�� ����

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

	virtual COLORREF** GetPixelList(void) { return m_collLayer->_pixel; }
	virtual float GetMapWidth()
	{
		if (this->m_tileLayerVector.empty()==true)
		{
			cout << "�ʿ� Ÿ�� ���� �ȵ�. ������ ȣ�� ����" << endl;
			return NULL;
		}
		return this->m_tileLayerVector[0]->layerImage->GetWidth();
	}
	virtual float GetMapHeight()
	{
		if (this->m_tileLayerVector.empty() == true)
		{
			cout << "�ʿ� Ÿ�� ���� �ȵ�. ������ ȣ�� ����" << endl;
			return NULL;
		}
		return this->m_tileLayerVector[0]->layerImage->GetHeight();
	};

	Scene() : isLoaded(false) {}
	virtual ~Scene() 
	{
		if (m_collLayer != nullptr)
		{
			m_collLayer->Release();
			SAFE_DELETE(m_collLayer);
		}
	}
};