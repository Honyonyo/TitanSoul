#include "Object.h"

class PixelCollision;
class Snowball :public Object
{
private:
	CImage* m_img;
	float m_angle;
	float m_speed;
	float m_imgRot;

	bool m_render;
	bool m_move;

	PixelCollision* m_pixelCollision;

public:
	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	void SetRender();
	void SetMove(D2D1_POINT_2F playerPos);

	virtual void Attack(eObjectKinds kinds);
	virtual void Hit(eObjectKinds kinds);

	bool GetIsRender() { return m_render; }

	Snowball(float centerX, float centerY);
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
	virtual void Hit(eObjectKinds kinds);	

	void Setting(float x, float y, int imgnum);

	Icicle(float x, float y, int imgnum);
	Icicle();
	~Icicle();
};