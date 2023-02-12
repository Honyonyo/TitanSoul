#pragma once

static GImage* _backBuffer = IMAGEMANAGER->AddBackBuffer("backBuffer", WINSIZE_X, WINSIZE_Y);

class GameNode
{
private : 
	HDC _hdc;

	bool _managerInit;
protected :
	PAINTSTRUCT _ps; //����Ʈ����ü

public:
	//�Լ��� ���������� ����Ǿ����� üũ
	//32��Ʈ wjdtn(������ �ƴ� ���� ����, ���� ����)
	virtual HRESULT Init(bool managerInit);
	virtual HRESULT Init(void);
	virtual void Release(void);
	virtual void Update(void);
	virtual void Render(void);

	LRESULT MainProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

	GImage* getBackBuffer(void) { return _backBuffer; }
	HDC getMemDC() { return _backBuffer->GetMemDC(); }
	//HDC���(ȭ��DC)
	HDC getHDC() { return _hdc; }


	GameNode() {};

	//����Ҹ���!
	virtual ~GameNode() {};
};

