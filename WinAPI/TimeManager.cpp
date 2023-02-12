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
		_timer = nullptr; // ���� ���ܼ� ������  Ÿ�̸ӿ��� ���� �� �����Ƿ� �õ����ڸ��� ���鸸������
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

	//_DEBUG ����� ����ϋ��� ����.
	// �������� �ϸ鼭 �׽�Ʈ�� �뵵�� ���ϴ�.
	// ���������� Ȯ���� �ϸ� �ȵǴ� ������ ��Ű��ȭ ��ų�� ���ϴ�.
	wstring wstr;
#ifdef _DEBUG 
	if (_timer != nullptr)
	{
		wstr = L"FramePerSec: " + to_wstring(_timer->getFrameRate());
		TEXTMANAGER->TextDraw(wstr.c_str(), "��Ʈ_20", { 10, 0, 500,10},RGB(255,255,255));
		
		wstr = L"ElapsedTime: " + to_wstring(_timer->getElapsedTime());
		TEXTMANAGER->TextDraw(wstr.c_str(), "��Ʈ_20", { 10, 20, 500,60}, RGB(255, 255, 255));
		
		wstr = L"WorldTime: " + to_wstring(_timer->getWorldTime());
		TEXTMANAGER->TextDraw(wstr.c_str(), "��Ʈ_20", { 10, 40, 500,90}, RGB(255, 255, 255));
	}

#else
	sprintf_s(str, "FramePerSec: %d", _timer->gerFrameRate());
	//TextOut(hdc, 0, 0, str, strlen(str));


#endif // _DEBUG

}
