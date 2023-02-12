#include "stdafx.h"
#include "PixelCollision.h"


HRESULT PixelCollision::Init(void)
{
	//m_collColor[eWall] = RGB(255, 226, 64);
	m_pixel = SCENEMANAGER->GetCurrentScenePixel();
	return S_OK;
};
void PixelCollision::Update()
{
	m_leftColl = false;
	m_topColl = false;
	m_rightColl = false;
	m_bottomColl = false;
	m_isColl = false;

	SetProvbox();

	if (m_pixel == nullptr)
	{
		m_pixel = SCENEMANAGER->GetCurrentScenePixel();
	}
	else if (m_pixel != SCENEMANAGER->GetCurrentScenePixel())
	{
		m_pixel = SCENEMANAGER->GetCurrentScenePixel();
	}

	LeftCheck();
	TopCheck();
	RightCheck();
	BottomCheck();

};
void PixelCollision::Render()
{

};
void PixelCollision::Release()
{
};

//eDiagonal일 때 
void PixelCollision::LeftCheck()
{
	for (int provLeft = m_provbox.top; provLeft <= m_provbox.bottom; provLeft++)
	{
		bool breakProveL = false;
		for (int i = 0; i < eCollisionSpecNumCount; i++)
		{
			if (m_pixel[m_provbox.left][provLeft] != m_collColor[i])
			{
				continue;
			}//색이 다를 때 : 다음루프로
			else
			{
				switch (i)
				{

				case eDiagonal:
					break;
				case eWall:
					m_leftColl =true;
					m_isColl = true;
					//밀어주기! 색이 다른 점이 나올 때까지 찾고, 다른 점이 나오면 거기에서부터 탐지width만큼 center를 옮긴다.
					for (int pushLeft = m_provbox.left; pushLeft <= m_provbox.left + m_provWidth / 2; pushLeft++)
					{
						if (m_pixel[pushLeft][provLeft] != m_collColor[i])
						{
							m_center->x = pushLeft + m_provWidth * 0.5f - m_centerDistanceX;
							break;
						}
					}
					breakProveL = true;
					break;
				default:
					cout << "픽셀충돌처리 케이스에 안걸림. 현재 컬러색상 " << m_pixel[m_provbox.left][provLeft] << endl;
				}

			}//색이 같을 때 : swicth에서 조건돌려주기
		}//end for eCollSpec

		if (breakProveL) break;
	}//end for provLeft
};

void PixelCollision::TopCheck()
{
	for (int provTop = m_provbox.left; provTop <= m_provbox.right; provTop++)
	{
		bool breakProveT = false;
		for (int i = 0; i < eCollisionSpecNumCount; i++)
		{
			if (m_pixel[provTop][m_provbox.top] != m_collColor[i])
			{
				continue;
			}//색이 다를 때 : 다음루프로
			else
			{
				switch (i)
				{
				case eDiagonal:
					break;
				case eWall:
					m_topColl = true;
					m_isColl = true;
					//밀어주기! 색이 다른 점이 나올 때까지 찾고, 다른 점이 나오면 거기에서부터 탐지width만큼 center를 옮긴다.
					for (int pushTop = m_provbox.top; pushTop <= m_provbox.top + m_provHeight / 2; pushTop++)
					{
						if (m_pixel[provTop][pushTop] != m_collColor[i])
						{
							m_center->y = pushTop + m_provHeight * 0.5f - m_centerDistanceY;
							break;
						}
					}
					breakProveT = true;
					break;
				default:
					cout << "픽셀충돌처리 케이스에 안걸림. 현재 컬러색상 " << m_pixel[provTop][m_provbox.top] << endl;
				}

			}//색이 같을 때 : swicth에서 조건돌려주기
		}//end for eCollSpec

		if (breakProveT) break;
	}//end for provTop
};

void PixelCollision::RightCheck()
{
	for (int provRight = m_provbox.top; provRight <= m_provbox.bottom; provRight++)
	{
		bool breakProveR = false;
		for (int i = 0; i < eCollisionSpecNumCount; i++)
		{
			if (m_pixel[m_provbox.right][provRight] != m_collColor[i])
			{
				continue;
			}//색이 다를 때 : 다음루프로
			else
			{
				switch (i)
				{
				case eDiagonal:
					break;
				case eWall:
					m_rightColl = true;
					m_isColl = true;
					//밀어주기! 색이 다른 점이 나올 때까지 찾고, 다른 점이 나오면 거기에서부터 탐지width만큼 center를 옮긴다.
					for (int pushRight = m_provbox.right; pushRight >= m_provbox.right - m_provWidth / 2; pushRight--)
					{
						if (m_pixel[pushRight][provRight] != m_collColor[i])
						{
							m_center->x = pushRight - m_provWidth * 0.5f - m_centerDistanceX;
							break;
						}
					}
					breakProveR = true;
					break;
				default:
					cout << "픽셀충돌처리 케이스에 안걸림. 현재 컬러색상 " << m_pixel[m_provbox.right][provRight] << endl;
				}

			}//색이 같을 때 : swicth에서 조건돌려주기
		}//end for eCollSpec

		if (breakProveR) break;
	}//end for provRight
};

void PixelCollision::BottomCheck()
{
	for (int provBottom = m_provbox.left; provBottom <= m_provbox.right; provBottom++)
	{
		bool breakProveB = false;
		for (int i = 0; i < eCollisionSpecNumCount; i++)
		{
			if (m_pixel[provBottom][m_provbox.bottom] != m_collColor[i])
			{
				continue;
			}//색이 다를 때 : 다음루프로
			else
			{
				switch (i)
				{
				case eDiagonal:
					break;
				case eWall:
					m_bottomColl = true;
					m_isColl = true;
					//밀어주기! 색이 다른 점이 나올 때까지 찾고, 다른 점이 나오면 거기에서부터 탐지width만큼 center를 옮긴다.
					for (int pushBottom = m_provbox.bottom; pushBottom >= m_provbox.bottom - m_provHeight / 2; pushBottom--)
					{
						if (m_pixel[provBottom][pushBottom] != m_collColor[i])
						{
							m_center->y = pushBottom - m_provHeight * 0.5f - m_centerDistanceY;
							break;
						}
					}
					breakProveB = true;
					break;
				default:
					cout << "픽셀충돌처리 케이스에 안걸림. 현재 컬러색상 " << m_pixel[provBottom][m_provbox.bottom] << endl;
				}

			}//색이 같을 때 : swicth에서 조건돌려주기
		}//end for eCollSpec

		if (breakProveB) break;
	}//end for provBottom
};



void PixelCollision::SetProvbox()
{
	m_provbox = RectMakeCenter(m_center->x + m_centerDistanceX, m_center->y + m_centerDistanceY, m_provWidth, m_provHeight);
};





PixelCollision::PixelCollision(D2D1_POINT_2F* center, float centerDistanceX, float centerDistanceY, float width, float height)
	:m_leftColl(false), m_topColl(false), m_rightColl(false), m_bottomColl(false), m_isColl(false)
{
	m_center = center;
	m_centerDistanceX = centerDistanceX;
	m_centerDistanceY = centerDistanceY;
	m_provWidth = width;
	m_provHeight = height;

	SetProvbox();
}
