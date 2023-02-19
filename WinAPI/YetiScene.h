#pragma once
#include "Scene.h"
#include "Yeti.h"
class YetiScene : public Scene
{
private:
	/*
	* 상속받은 변수 *
	tileSheetList mapTileSheetList;
	vector<tagTile*> m_tileLayerVector;
	vector<tagTile*>::iterator m_iterTileLayerVector;
	*/
	string text[5];
	Yeti* m_yeti;
	RECT m_floorGate;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);


	virtual float GetMapWidth()
	{
		if (this->m_tileLayerVector.empty() == true)
		{
			cout << "맵에 타일 아직 안들어감. 사이즈 호출 실패" << endl;
			return NULL;
		}
		return this->m_tileLayerVector[0]->layerImage->GetWidth();
	}
	virtual float GetMapHeight()
	{
		cout << this << endl;
		if (this->m_tileLayerVector.empty() == true)
		{
			cout << "맵에 타일 아직 안들어감. 사이즈 호출 실패" << endl;
			return NULL;
		}
		return this->m_tileLayerVector[0]->layerImage->GetHeight();
	};

	YetiScene();
	~YetiScene();
};

