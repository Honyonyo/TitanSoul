#pragma once
#include "GameNode.h"

class MainGame : public GameNode
{
public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);
	void Render(void);

	MainGame() {}
	~MainGame() {}
};
