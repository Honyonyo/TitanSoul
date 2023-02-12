#pragma once

static GImage* _backBuffer = IMAGEMANAGER->AddBackBuffer("backBuffer", WINSIZE_X, WINSIZE_Y);

class GameNode
{
private : 
	HDC _hdc;

	bool _managerInit;
protected :
	PAINTSTRUCT _ps; //페인트구조체

public:
	//함수가 성공적으로 실행되었는지 체크
	//32비트 wjdtn(음수가 아닌 값은 성공, 음수 실패)
	virtual HRESULT Init(bool managerInit);
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	LRESULT MainProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	GImage* getBackBuffer(void) { return _backBuffer; }
	HDC getMemDC() { return _backBuffer->GetMemDC(); }
	//HDC얻기(화면DC)
	HDC getHDC() { return _hdc; }


	GameNode() {};

	//가상소멸자!
	virtual ~GameNode() {};
};

