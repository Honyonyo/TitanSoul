#include "Scene.h"
#include "YetiScene.h"
#include "SludgeheartScene.h"

class FrostMountainsField : public Scene
{
private:
	

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

};

class FrostMountains : public Scene
{
private:
	YetiScene* m_yetiScene;
	SludgeheartScene* m_sludgeheartScene;
	FrostMountainsField* m_field;
	Scene* m_currentArea;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	virtual COLORREF** GetPixelList(void) 
	{ return m_currentArea->GetPixelList(); }
	virtual float GetMapWidth()
	{
		return m_currentArea->GetMapWidth();
	};
	virtual float GetMapHeight()
	{
		return m_currentArea->GetMapHeight();
	}
};