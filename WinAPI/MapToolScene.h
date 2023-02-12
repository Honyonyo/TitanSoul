#pragma once
#include "GameNode.h"

class MapToolScene : public GameNode
{
public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

};