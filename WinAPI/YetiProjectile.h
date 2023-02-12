#include "Object.h"
class Snowball
{
private:

public:

};

class Icicle : public Object
{
private:
	CImage* m_img;
	float m_shadowScale;

	D2D1_POINT_2F m_imageCenter;
	bool m_imageChange;
	float m_lifetime;
	int m_imgNum;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);	//날 때린놈의 종류에 따라서 다른 반응 보여주기

	void Setting(float x, float y, int imgnum);

	Icicle(float x, float y, int imgnum);
	Icicle();
	~Icicle();
};