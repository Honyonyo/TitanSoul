#include "stdafx.h"
#include "Camera.h"
Camera::Camera(float scale) :
	m_camRot(0.f),
	m_scaleIncrease(false), m_move(false), m_moveSpeed(0.f),
	m_shaking(false), m_shakingLv(0)
{
	m_camScale = scale;
	m_camWidth = WINSIZE_X / scale;
	m_camHeight = WINSIZE_Y / scale;
	m_renderT_2F = { 0.f,0.f };
	if (PLAYER != nullptr) SetCenter(PLAYER->GetPointF());
	m_mapWidth = SCENEMANAGER->GetCurrentSceneWidth();
	m_mapHeight = SCENEMANAGER->GetCurrentSceneHeight();

	SetRenderT();
}
Camera::~Camera()
{
}

void Camera::SetRenderT()
{
	m_camWidth = WINSIZE_X / m_camScale;
	m_camHeight = WINSIZE_Y / m_camScale;

	//흔들림, 이동같은 특별한 효과가 있지 않은 경우와 일반적인 경우를 나눠야함
	//맵 사이즈가 카메라 사이즈보다 작을 경우 , 클경우를 나눠서 해야함
	//가로 세로 나눠서

	if (!m_move)
	{//가로 T위치 잡기
		if (m_mapWidth < m_camWidth)	//맵사이즈보다 카메라사이즈(화면출력너비)가 넓은 경우
		{
			m_renderT_2F.x = -(m_camWidth - m_mapWidth) / 2;
		}
		else//일반적인 경우
		{
			m_renderT_2F.x = m_centerP_2F.x - (m_camWidth / 2);
			//캠이 맵에서 벗어난 경우
			if (m_renderT_2F.x < 0) m_renderT_2F.x = 0;
			else if (m_renderT_2F.x + m_camWidth > m_mapWidth) m_renderT_2F.x = m_mapWidth - m_camWidth;
		}

		//세로 T위치 잡기
		if (m_mapHeight < m_camHeight)		//맵 사이즈보다 카메라사이즈(화면출력높이)가 더 긴 경우
		{
			m_renderT_2F.y = -(m_mapHeight - m_camHeight) / 2;
		}
		else
		{
			m_renderT_2F.y = m_centerP_2F.y - m_camHeight / 2;
			//캠이 맵에서 벗어난 경우
			if (m_renderT_2F.y < 0) m_renderT_2F.y = 0;
			else if (m_renderT_2F.y + m_camHeight > m_mapHeight)
				m_renderT_2F.y = m_mapHeight - m_camHeight;
		}
	}//end if not camera move
	else//카메라 무브가 들어갈 경우
	{
		m_renderT_2F = { m_centerP_2F.x - m_camWidth / 2, m_centerP_2F.y - m_camHeight / 2 };
	}

	if (m_shaking)
	{
		//##
	}

}

void Camera::Init()
{
	//if (m_mapWidth == NULL)m_mapWidth = SCENEMANAGER->GetCurrentSceneWidth();
	//if (m_mapHeight == NULL)m_mapHeight = SCENEMANAGER->GetCurrentSceneHeight();

	if (PLAYER != nullptr) SetCenter(PLAYER->GetPointF().x, PLAYER->GetPointF().y);
}

void Camera::Update()
{
	if (!m_move)
		SetCenter(PLAYER->GetPointF());

	if (m_scaleIncrease)
	{
		m_camScale += 0.008;
		if (m_camScale > IMAGE_SCALE * 1.2)
		{
			m_camScale = IMAGE_SCALE * 1.2;
		}
	//	cout << m_camScale << endl;
	}

	else if (m_camScale > IMAGE_SCALE)
	{
		m_camScale -= 0.01;
		if (m_camScale < IMAGE_SCALE)
		{
			m_camScale = IMAGE_SCALE;
		}
	}


	SetRenderT();
}
void Camera::Release()
{

};
void Camera::Render()
{

}
