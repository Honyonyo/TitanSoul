#include "stdafx.h"
#include "GameNode.h"

HRESULT GameNode::Init(void) {

	return S_OK;
}

HRESULT GameNode::Init(bool managerInit)
{
    _hdc = GetDC(_hWnd);
    _managerInit = managerInit;

    if(managerInit) {
        SetTimer(_hWnd, 0, 10, NULL);

        RND->Init();
        GAMEMANAGER->Init();
        TIMEMANAGER->Init();
        KEYMANAGER->Init();
        IMAGEMANAGER->Init();
        TEXTMANAGER->Init();
        SOUNDMANAGER->Init();  
        SCENEMANAGER->Init();
        OBJECTMANAGER->Init();
    }
        return S_OK;
}

void GameNode::Release(void)
{
    if(_managerInit) {
        KillTimer(_hWnd, 0);

        RND->ReleaseSingleton();
        KEYMANAGER->ReleaseSingleton();
        IMAGEMANAGER->ReleaseSingleton();
        SOUNDMANAGER->ReleaseSingleton();
        TIMEMANAGER->Release();
        TIMEMANAGER->ReleaseSingleton();
        SCENEMANAGER->Release();
        OBJECTMANAGER->Release();
    }
}

void GameNode::Update(void) {
}

void GameNode::Render(void) {
}

LRESULT GameNode::MainProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    HDC hdc;        //�ڵ�dc_ GDI�ȿ��ִµ�. png�� �ȵǰ� bmp�� �޾���. GDI+������ϸ� PNG�� ����� �� �ִ�.
    switch (iMsg) {
    case WM_TIMER:
        this->Update();
        break;

    case WM_PAINT:          //��¿� ���� ��� ���� ����Ѵ�(����,�׸�,���� ��� ȭ�鿡 ���̴� ��� ��)
        _ps.hdc = BeginPaint(hWnd, &_ps);
        this->Render();
        EndPaint(hWnd, &_ps);
        break;

    case WM_MOUSEMOVE:
        _ptMouse.x = LOWORD(lParam);
        _ptMouse.y = HIWORD(lParam);
        break;

    case WM_LBUTTONDOWN:    //��Ŭ���߻�
        break;

    case WM_LBUTTONUP:
        break;

    case WM_RBUTTONDOWN:    //��Ŭ�� �߻� 

        break;

    case WM_KEYDOWN:    //Ű������Ȳ���� �߻�
        switch (wParam) {
        case VK_ESCAPE:
            PostMessage(hWnd, WM_DESTROY, 0, 0);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return (DefWindowProc(hWnd, iMsg, wParam, lParam));

}
