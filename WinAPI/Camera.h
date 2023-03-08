#pragma once
static class Camera
{
private:
	float m_camScale;
	float m_camRot;
	D2D1_POINT_2F m_centerP_2F;
	D2D1_POINT_2F m_renderT_2F;
	float m_camWidth;
	float m_camHeight;
	float m_mapWidth;
	float m_mapHeight;

	bool m_scaleIncrease;

	bool m_moveToRT;
	bool m_moving;
	float m_moveSpeed;
	D2D1_POINT_2F m_moveTarget;

	bool m_shaking;
	bool m_shakingLeft;
	float m_shakingLv;
	float m_shakingSpeed;
	float m_shakeChange;

	void SetRenderT();
	void SetCameraCenter();
	void SetShaking();

public:
	void Init();
	void Update();
	void Render();
	void Release();

#pragma region 접근자 지정자
	float GetScale() { return m_camScale; }
	POINT GetRenderTargetPOINT() { return { (long)m_renderT_2F.x,(long)m_renderT_2F.y }; }
	D2D1_POINT_2F GetRenderTargetPoint_2F() { return { m_renderT_2F.x, m_renderT_2F.y }; }
	float GetRenderTargetX() { return m_renderT_2F.x; }
	float GetRenderTargetY() { return m_renderT_2F.y; }
	POINT GetCenterPOINT() { return { (long)m_centerP_2F.x, (long)m_centerP_2F.y }; }
	D2D1_POINT_2F GetCenterPoint_2F() { return m_centerP_2F; }
	float GetCenterX() { return m_centerP_2F.x; }
	float GetCenterY() { return m_centerP_2F.y; }

	void SetMapSize(float mapW, float mapH) { m_mapWidth = mapW; m_mapHeight = mapH; }
	void SetScale(float scale) { m_camScale = scale; }
	void SetCenter(float x, float y) { m_centerP_2F.x = x;  m_centerP_2F.y = y; }
	void SetCenter(D2D1_POINT_2F center) { m_centerP_2F = center; }
	//카메라 흔들림 설정
	//흔들림 스위치, 흔들림 강도(초당 몇픽셀), 속도
	//		SetCameraShaking(true, 2, 20); 요정도가 요동둉
	void SetCameraShaking(bool shakingOnOff, float shakingLv, float shakingSpeed = 1.f)
	{
		m_shaking = shakingOnOff; 
		m_shakingLv = shakingLv;
		m_shakingSpeed = shakingSpeed;
	}
	//카메라 흔들림 끄기
	void SetCameraShakingOff()
	{
		m_shaking = false; 
		m_shakingLeft = false;
		m_shakingLv = 0;
		m_shakeChange = 0;
		m_shakingSpeed = 1.f;
		SetCameraMove(PLAYER->GetPointF(), false, true, 1);
	}

	void SetScaleIncease(bool onOff)
	{
		m_scaleIncrease = onOff;
	}

	//카메라 이동 옵션
	void SetCameraMove(D2D1_POINT_2F renderT, bool move, bool moving = true, float moveSpeed = 1.f)
	{
		//cout << "SetCameraMove! renderT : " << renderT.x << ", " << renderT.y << endl;
		m_moveTarget = renderT;
		m_moveToRT = move;
		m_moving = moving;
		m_moveSpeed = moveSpeed;
	}

#pragma endregion
	Camera(float scale = IMAGE_SCALE);
	~Camera();
};