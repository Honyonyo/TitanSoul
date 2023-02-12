#include "stdafx.h"
#include "MainGame.h"
//#include "MainGameMap.h"

#pragma region 윈도우API
/*
* API (Application Programming Interface)
    (//게임 시장점유율이 win>>>>>>>>>mac이라서.. winAPI위주로 겅부하는게 시장성 높음음)

    ★- 운영체제가 응용프로그램 개발을 위해 제공하는 서브루틴 또는 함수의 집합

    - 명령어의 집합으로 어플리케이션 프로그램에서 오퍼레이팅 시스템의 기본적인 기능을 사용할 필요가 있을 경우 API에서 명령어 호출이 진행된다.

    하드웨어---------------운영체제(win)-------------응용프로그램
    (API함수)              ...            
    ★▶ 장 점
    - 운영체제가 같으면 같은 기능의 집합을 사용하기에 확장성 및 연동성, 그리고 유지보수가 편리함.
    - 윈도우즈 != 리눅스 
    - 윈도우즈에서 지원하는 다양한 라이브러리 지원 가능
        커스텀 추가 라이브러리를 추가해서 단일플랫폼으로 쓰지 않고 부족한점 보완하고 필요에 따라 다양한 기능을 추가하는거야!

    ★▶ 단 점
    - 플랫폼 고착화 (오래 전 C로 만들어져서 최신 하드에는 맞지 않을 수 있다. (gpu 등등))
        └라이브러리 가져와서 사용하면 해결 빠밤!┘
    
    ▶ API구조
    ★- 크게 진입점, ★메세지 루프, ★Window Procedure로 나뉜다.
        프로시저에서 메세지를 발생시키고,
        반복문을 통해 전달해준다.
        실행 > 인스턴스 > 진입점 > 확인(컴파일러가 작업) > 프로시저 메세지 반복문 메세지루프로 전달, 메세지 get인지 pick?인지에 따라 윈도우즈로 전달하거나 계속소통하거나
            cpu클럭에 넣기       창 띄우기 위한 세팅     프로시저에 작성된 문을 메세지 형태로 읽어들인다

*/
#pragma region 

HINSTANCE _hinstance;                               // 현재 인스턴스입니다.
HWND _hWnd;                                         //윈도우핸들 : 윈도우 창
POINT _ptMouse = { 0,0 };

MainGame* _mg;

// # 함수 전방선언 #


LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

//메 인 함 수! 듀듕 
int APIENTRY WinMain(HINSTANCE hInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR     lpszCmdParam,
                    int       nCmdShow)
{
    _mg = new MainGame;
    _hinstance = hInstance;

    WNDCLASS wndClass;

    //메모리
    wndClass.cbClsExtra = 0;                                                    //클래스 여분 메모리
    wndClass.cbWndExtra = 0;                                                    //윈도우 여분 메모리
    //화면
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);               //백그라운드. 페이드 인아웃이 들어갈 수 있도록 여지를 준 get stock object
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);                             //마우스커서. 아이콘 이미지 기본화살표 그냥 쓸꺼양
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);                           //아이콘. 안쓸건데, 기본지원으로 그냥 세팅해둬.
    //윈도우 정보, 속성
    wndClass.hInstance = hInstance;                                             //윈도우를 소요한 프로그램의 식별자 정보
    wndClass.lpfnWndProc = (WNDPROC)WndProc;                                    //윈도우 프로시져(현재 프로젝트에서 사용할)
    wndClass.lpszClassName = WIN_NAME;                                        //클래스 이름 (윈도우 클래스 식별자 정보)
    
    wndClass.lpszMenuName = NULL;                                               //메뉴 이름. 지금 안쓸꺼야~
    wndClass.style = CS_HREDRAW | CS_VREDRAW;                                   //윈도우 스타일 (윈도우 다시그리기 정보)

    //1-2. 윈도우 클래스 등록
    RegisterClass(&wndClass);

#ifdef FULL_SCREEN
    DEVMODE dm;

    ZeroMemory(&dm, sizeof(DEVMODE));
    dm.dmSize = sizeof(DEVMODE);
    dm.dmBitsPerPel = 32;       //32비트 트루컬러
    dm.dmPelsWidth = 1980;
    dm.dmPelsHeight = 1080;
    dm.dmDisplayFrequency = 60; //주사율 60Hz

    dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;

    //화면이 종료되면 원래 해상도로 복귀
    if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
        ChangeDisplaySettings(&dm, 0);
    }

    //1-3. 화면에 보여줄 윈도우 창 생성
    _hWnd = CreateWindow    
    (
        WIN_NAME,     //윈도우 클래스 식별자
        WIN_NAME,     //윈도우 타이틀 바 이름
        WINSTYLE,    //윈도우 스타일
        WINSTART_X,            //윈도우 화면 X좌표. 내가 만든 창이 뜨는 모니터 화면 위치
        WINSTART_Y,            //윈도우 화면 Y좌표. 내가 만든 창이 뜨는 모니터 화면 위치
        WINSIZE_X,            //윈도우 화면 가로크기
        WINSIZE_Y,            //윈도우 화면 세로크기
        NULL,//부모 윈도우 지정 여부 (없으면 NULL, 그치만안정성높은건 GetDesktopWindow())
        (HMENU)NULL,    //메뉴 핸들
        hInstance,      //인스턴스 지정
        NULL            //윈도우의 자식 윈도우를 생성하면 지정하고 아니라면 NULL_아마 메세지창..?경고창/
                            //필요에 의해서 사용. 지금은 null
    );

    setWindowSize(WINSTART_X, WINSTART_Y, WINSIZE_X, WINSIZE_Y);

#else

    //1-3. 화면에 보여줄 윈도우 창 생성
    _hWnd = CreateWindow    //사용하는 일이 많으므로 친구하자^^
    (
        WIN_NAME,     //윈도우 클래스 식별자
        WIN_NAME,     //윈도우 타이틀 바 이름
        WINSTYLE,    //윈도우 스타일
        WINSTART_X,            //윈도우 화면 X좌표. 내가 만든 창이 뜨는 모니터 화면 위치
        WINSTART_Y,            //윈도우 화면 Y좌표. 내가 만든 창이 뜨는 모니터 화면 위치
        WINSIZE_X,            //윈도우 화면 가로크기
        WINSIZE_Y,            //윈도우 화면 세로크기
        NULL,//부모 윈도우 지정 여부 (없으면 NULL, 그치만안정성높은건 GetDesktopWindow())
        (HMENU)NULL,    //메뉴 핸들
        hInstance,      //인스턴스 지정
        NULL            //윈도우의 자식 윈도우를 생성하면 지정하고 아니라면 NULL_아마 메세지창..?경고창/
                            //필요에 의해서 사용. 지금은 null
    );

    setWindowSize(WINSTART_X, WINSTART_Y, WINSIZE_X, WINSIZE_Y);

#endif

    //1-4. 화면에 윈도우 창 보여주기
    ShowWindow(_hWnd, nCmdShow);

    //메인게임 클래스 초기화 실패시 종료
    if(FAILED(_mg->Init()))
    {
        return 0;
    }
    MSG message;

//게임용~
    while (true) {
        if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT)break;
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        else
        {
            TIMEMANAGER->Update(120.f);
            _mg->Update();
            _mg->Render();
        }
    }


    //while (GetMessage(&message, 0, 0, 0))
    //{
    //        TranslateMessage(&message);
    //        DispatchMessage(&message);
    //}

    _mg->Release();
    //윈도우 클래스 등록 해제
    UnregisterClass(WIN_NAME, hInstance);

    return (int)message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

    return _mg->MainProc(hWnd, iMsg, wParam, lParam);
}

void setWindowSize(int x, int y, int width, int height)
{
    RECT rc = { 0,0,width, height };
    //실제 윈도우 크기 조정
    //AdjustWindowRect(대상 객체, 사용할윈도우스타일, 메뉴 여부)
    AdjustWindowRect(&rc, WINSTYLE, false);

    //얻어온 렉트정보로 윈도우 사이즈 셋팅
    SetWindowPos(_hWnd, NULL, x, y,
        (rc.right - rc.left),
        (rc.bottom - rc.top),
        (SWP_NOZORDER | SWP_NOMOVE));
}
//zorder 겹쳐있는 뒤에거를 어떻게 처리할지
