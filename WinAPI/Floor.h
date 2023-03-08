#pragma once
#include "Scene.h"
class Colossus;
class Floor : public Scene
{
private:
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

