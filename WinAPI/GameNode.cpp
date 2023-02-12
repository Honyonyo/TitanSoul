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
    HDC hdc;        //핸들dc_ GDI안에있는데. png는 안되고 bmp만 받아줌. GDI+를사용하면 PNG를 사용할 수 있다.
    switch (iMsg) {
    case WM_TIMER:
        this->Update();
        break;

    case WM_PAINT:          //출력에 관한 모든 것을 담당한다(문자,그림,도형 등등 화면에 보이는 모든 것)
        _ps.hdc = BeginPaint(hWnd, &_ps);
        this->Render();
        EndPaint(hWnd, &_ps);
        break;

    case WM_MOUSEMOVE:
        _ptMouse.x = LOWORD(lParam);
        _ptMouse.y = HIWORD(lParam);
        break;

    case WM_LBUTTONDOWN:    //좌클릭발생
        break;

    case WM_LBUTTONUP:
        break;

    case WM_RBUTTONDOWN:    //우클릭 발생 

        break;

    case WM_KEYDOWN:    //키눌린상황에서 발생
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
