#include "Stdafx.h"
#include "TimeManager.h"

HRESULT TimeManager::Init(void)
{
	_timer = new Timer;
	_timer->Init();
	return S_OK;
}

void TimeManager::Release(void)
{
	if (_timer != nullptr)
	{
		SAFE_DELETE(_timer);
		_timer = nullptr; // 문제 생겨서 꺼지면  타이머에는 값이 들어가 있으므로 시동되자마자 공백만들어버림
	}
}

void TimeManager::Update(float lock)
{
	if (_timer != nullptr)
	{
		_timer->tick(lock);
	}
}

void TimeManager::Render()
{
	//char str[256];
	//SetBkMode(hdc, TRANSPARENT);
	//SetTextColor(hdc, RGB(255, 255, 255));

	//_DEBUG 디버그 모드일떄만 동작.
	// ㄴ개발을 하면서 테스트의 용도가 강하다.
	// ㄴ유저들이 확인을 하면 안되는 정보를 패키지화 시킬때 편하다.
	wstring wstr;
#ifdef _DEBUG 
	if (_timer != nullptr)
	{
		wstr = L"FramePerSec: " + to_wstring(_timer->getFrameRate());
		TEXTMANAGER->TextDraw(wstr.c_str(), "도트_20", { 10, 0, 500,10},RGB(255,255,255));
		
		wstr = L"ElapsedTime: " + to_wstring(_timer->getElapsedTime());
		TEXTMANAGER->TextDraw(wstr.c_str(), "도트_20", { 10, 20, 500,60}, RGB(255, 255, 255));
		
		wstr = L"WorldTime: " + to_wstring(_timer->getWorldTime());
		TEXTMANAGER->TextDraw(wstr.c_str(), "도트_20", { 10, 40, 500,90}, RGB(255, 255, 255));
	}

#else
	sprintf_s(str, "FramePerSec: %d", _timer->gerFrameRate());
	//TextOut(hdc, 0, 0, str, strlen(str));


#endif // _DEBUG

}
