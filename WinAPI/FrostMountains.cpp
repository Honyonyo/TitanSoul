#include "stdafx.h"
#include "FrostMountains.h"

#pragma region Field
HRESULT FrostMountainsField::Init(void)
{
	return S_OK;
}

void FrostMountainsField::Release(void)
{

}

void FrostMountainsField::Update(void)
{

}

void FrostMountainsField::Render(void)
{

}
#pragma endregion

#pragma region FrostMountains
HRESULT FrostMountains::Init(void)
{
	m_yetiScene = new YetiScene;
	m_field = new FrostMountainsField;
	m_yetiScene->Init();
	m_field->Init();
	m_currentArea = m_field;	//지역 첫 진입점은 필드일것이 확실하다고 생각함...제발
	//player 위치 씬 받아와서 Init 위치 잡아주기

	//#테스트용
	m_currentArea = m_yetiScene;
	return S_OK;
}

void FrostMountains::Release(void)
{
	m_yetiScene->Release();
	m_field->Release();
	m_currentArea = nullptr;
}

void FrostMountains::Update(void)
{
	if (m_currentArea != nullptr)
	{
		m_currentArea->Update();
	}
}

void FrostMountains::Render(void)
{
	if (m_currentArea != nullptr)
	{
		m_currentArea->Render();
	}
}
#pragma endregion