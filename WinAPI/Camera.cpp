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

	//��鸲, �̵����� Ư���� ȿ���� ���� ���� ���� �Ϲ����� ��츦 ��������
	//�� ����� ī�޶� ������� ���� ��� , Ŭ��츦 ������ �ؾ���
	//���� ���� ������

	if (!m_move)
	{//���� T��ġ ���
		if (m_mapWidth < m_camWidth)	//�ʻ������ ī�޶������(ȭ����³ʺ�)�� ���� ���
		{
			m_renderT_2F.x = -(m_camWidth - m_mapWidth) / 2;
		}
		else//�Ϲ����� ���
		{
			m_renderT_2F.x = m_centerP_2F.x - (m_camWidth / 2);
			//ķ�� �ʿ��� ��� ���
			if (m_renderT_2F.x < 0) m_renderT_2F.x = 0;
			else if (m_renderT_2F.x + m_camWidth > m_mapWidth) m_renderT_2F.x = m_mapWidth - m_camWidth;
		}

		//���� T��ġ ���
		if (m_mapHeight < m_camHeight)		//�� ������� ī�޶������(ȭ����³���)�� �� �� ���
		{
			m_renderT_2F.y = -(m_mapHeight - m_camHeight) / 2;
		}
		else
		{
			m_renderT_2F.y = m_centerP_2F.y - m_camHeight / 2;
			//ķ�� �ʿ��� ��� ���
			if (m_renderT_2F.y < 0) m_renderT_2F.y = 0;
			else if (m_renderT_2F.y + m_camHeight > m_mapHeight)
				m_renderT_2F.y = m_mapHeight - m_camHeight;
		}
	}//end if not camera move
	else//ī�޶� ���갡 �� ���
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
