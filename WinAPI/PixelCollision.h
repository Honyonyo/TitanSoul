class PixelCollision
{
	enum eCollisionSpec
	{
		eWall = 0,
		eDiagonal,

		eCollisionSpecNumCount
	};
private:
	RECT m_provbox;
	float m_centerDistanceX, m_centerDistanceY;
	float m_provWidth, m_provHeight;

	D2D1_POINT_2F* m_center;

	const COLORREF m_collColor[eCollisionSpecNumCount]
		= 
	{ 
		RGB(255, 226, 64),	//Wall
		RGB(17,240,150)		//Diagonal
	};
	COLORREF** m_pixel;


	bool m_leftColl;
	bool m_topColl;
	bool m_rightColl;
	bool m_bottomColl;
	bool m_isColl; 

public:
	HRESULT Init(void);
	void Update();
	void Render();
	void Release();

	void SetProvbox();

	void LeftCheck();
	void TopCheck();
	void RightCheck();
	void BottomCheck();
	bool GetLeftColl() { return m_leftColl; }
	bool GetTopColl() { return m_topColl; }
	bool GetRightColl() { return m_rightColl; }
	bool GetBottomColl() { return m_bottomColl; }
	bool GetIsColl() { return m_isColl; }
	void ResetColl() {
		m_leftColl=false;
		m_topColl=false;
		m_rightColl=false;
		m_bottomColl=false;
		m_isColl=false;	
	}

	PixelCollision(D2D1_POINT_2F* center, float centerDistanceX, float centerDistanceY, float width, float height);
	~PixelCollision()
	{}
};