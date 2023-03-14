#include "stdafx.h"
#include "Camera.h"
void Camera::SetCameraShakingOff()
{
	m_shaking = false;
	m_shakingLeft = false;
	m_shakingLv = 0;
	m_shakeChange = 0;
	m_shakingSpeed = 1.f;
	m_shakingTime = 0.f;
	SetCameraMove(PLAYER->GetPointF(), false, false);
}

Camera::Camera() :
	m_scaleIncrease(false), m_moveToRT(false), m_moving(false), m_moveSpeed(0.f),
	m_shakingLeft(false), m_shaking(false), m_shakingSpeed(1.f), m_shakingLv(0), m_shakeChange(0), m_shakingTime(0.f)
{
	m_camScale = IMAGE_SCALE;
	m_camWidth = WINSIZE_X / IMAGE_SCALE;
	m_camHeight = WINSIZE_Y / IMAGE_SCALE;
	m_renderT_2F = { 0.f,0.f };
	if (PLAYER != nullptr) SetCenter(PLAYER->GetPointF());
	m_mapWidth = SCENEMANAGER->GetCurrentSceneWidth();
	m_mapHeight = SCENEMANAGER->GetCurrentSceneHeight();

	SetRenderT();
}
Camera::~Camera()
{
}

void Camera::Init()
{
	if (PLAYER != nullptr) SetCenter(PLAYER->GetPointF().x, PLAYER->GetPointF().y);
}

void Camera::Update()
{
	SetCameraScale();
	SetCameraMoveCenter();
	SetShaking();
	SetRenderT();
}

void Camera::Release()
{

};
void Camera::Render()
{

}

void Camera::SetCameraScale()
{
	if (m_scaleIncrease)
	{
		m_camScale += 0.008;
		if (m_camScale > IMAGE_SCALE * 1.2)
		{
			m_camScale = IMAGE_SCALE * 1.2;
		}
	}

	else if (m_camScale > IMAGE_SCALE)
	{
		m_camScale -= 0.01;
		if (m_camScale < IMAGE_SCALE)
		{
			m_camScale = IMAGE_SCALE;
		}
	}
}

void Camera::SetRenderT()
{
	m_camWidth = WINSIZE_X / m_camScale;
	m_camHeight = WINSIZE_Y / m_camScale;

	//가로 T위치 잡기
	if (m_mapWidth < m_camWidth)	
		//맵사이즈보다 카메라사이즈(화면출력너비)가 넓은 경우
	{
		m_renderT_2F.x = -(m_camWidth - m_mapWidth) / 2 + m_shakeChange;
	}
	else//일반적인 경우
	{
		m_renderT_2F.x = m_centerP_2F.x - (m_camWidth / 2);
		//캠이 맵에서 벗어난 경우
		if (m_renderT_2F.x < 0) m_renderT_2F.x = 0;
		else if (m_renderT_2F.x + m_camWidth > m_mapWidth) 
			m_renderT_2F.x = m_mapWidth - m_camWidth + m_shakeChange;
	}

	//세로 T위치 잡기
	if (m_mapHeight < m_camHeight)		
		//맵 사이즈보다 카메라사이즈(화면출력높이)가 더 긴 경우
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
}

void Camera::SetCameraMoveCenter()
{
	if (m_moveToRT)
	{
		if (m_moving)
		{
			if (m_centerP_2F.x != m_moveTarget.x
				&& m_centerP_2F.y != m_moveTarget.y)
			{
				if (MY_UTIL::getDistance(m_centerP_2F.x, m_centerP_2F.y, m_moveTarget.x, m_moveTarget.y) < m_moveSpeed)
				{
					SetCenter(m_moveTarget);
					m_moving = false;
				}
				else
				{
					float angle = 
						MY_UTIL::getAngle(m_centerP_2F.x, m_centerP_2F.y, m_moveTarget.x, m_moveTarget.y);
					m_centerP_2F.x += cosf(angle) * m_moveSpeed*DELTA_TIME;
					m_centerP_2F.y -= sinf(angle) * m_moveSpeed*DELTA_TIME;
				}
			}
			else m_moving = false;
		}//end moving
		else
		{
			SetCenter(m_moveTarget);
		}
	}
	//movTRT꺼져있어도 moving인 경우 카메라 위치 확인해서 캐릭터로 옮겨주기
	else
	{
		if (m_moving)
		{
			if (m_centerP_2F.x != PLAYER->GetPointF().x
				&& m_centerP_2F.y != PLAYER->GetPointF().y)
			{
				if (MY_UTIL::getDistance(m_centerP_2F.x, m_centerP_2F.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y) < m_moveSpeed)
				{
					SetCenter(PLAYER->GetPointF());
					m_moving = false;
				}
				else
				{
					float angle = MY_UTIL::getAngle(m_centerP_2F.x, m_centerP_2F.y, PLAYER->GetPointF().x, PLAYER->GetPointF().y);
					m_centerP_2F.x += cosf(angle);
					m_centerP_2F.y -= sinf(angle);
				}
			}
			else m_moving = false;
		}
		//moving
		else
		{
			SetCenter(PLAYER->GetPointF());
		}
	}
}

void Camera::SetShaking()
{
	m_shakingTime -= DELTA_TIME;
	if (m_shakingTime <= 0)
	{
		SetCameraShakingOff();
	}
	if (!m_shaking) return;
	if (m_shakingLeft)
	{
		m_shakeChange -= m_shakingLv * m_shakingSpeed * DELTA_TIME;
		if (m_shakeChange <= -m_shakingLv)
		{
			m_shakingLeft = false; 
		}
	}
	else
	{
		m_shakeChange += m_shakingLv * m_shakingSpeed * DELTA_TIME;
		if (m_shakeChange >= m_shakingLv) 
		{
			m_shakingLeft = true;
		}

	}
	m_centerP_2F.x += m_shakeChange;
}