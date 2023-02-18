#pragma once
#include "Scene.h"
class IntroScene :public Scene
{
private:
	CImage* m_UIimage[3];
	float m_alpha;
	float m_delay;

	int m_imageNumber;

public:
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);
	void SceneChange(void);


	IntroScene();
	~IntroScene();
};

