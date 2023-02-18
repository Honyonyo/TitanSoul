#pragma once
#include "Scene.h"
#include "Sludgeheart.h"
class SludgeheartScene : public Scene
{
private:
	Sludgeheart* m_sludgeheart;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	SludgeheartScene();
	~SludgeheartScene();
};

