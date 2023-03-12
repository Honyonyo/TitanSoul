#pragma once
//PreCompiledHeader PCH

//4005 : DXGI등 그래픽 인터페이스에서 매크로 관련하여 많이 출력되는 경고
//프로젝트 속성 고급에서 설정 가능
#pragma warning(disable : 4005)
#define _CRT_SECURE_NO_WARNINGS

//프로그램에서 지원할 OS에 따라 Windows헤더이서 코드에 포함될 함수, 상수등을 제어하는데 사용이 된다.
#include <SDKDDKVer.h>

//거의 사용하지 않는 내용은 Windows헤더에서 제외
#define WIN32_LEAN_AND_MEAN
//GDI+를 사용하려면 헤더파일 꺼내와야한다.

//!Windows헤더파일
#include <Windows.h>
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#pragma comment (lib, "msimg32.lib")
#pragma comment (lib ,"winmm.lib")
//#pragma comment (lib ,"FMOD/lib/x64/FOMD/fmodex_vc.lib")
#pragma comment (lib ,"FMOD/lib/FMOD/fmodex64_vc.lib")
#pragma comment(lib, "JsonCPP/lib_json/json_vc71_libmtd.lib")

//! 외부 헤더파일
#include "FMOD\inc\FMOD\fmod.hpp"
#include "JsonCPP/include/json/json.h"

//!C 런타임 헤더파일
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

//!C++런타임 헤더파일
#include <iostream>
#include <fstream>

#include <chrono>
#include <random>
#include <vector>
#include <list>
#include <map>
#include <queue>

#include <unordered_map>
#include <string>
#include <bitset>

using std::vector;
using std::list;
using std::map;
using std::queue;
using std::string;
using std::bitset;

using std::cout;
using std::endl;

#include <cassert>	//추후 설명해주세요

#pragma region D2d1FactoryHeader
#pragma comment (lib, "msimg32.lib") 

#include <d2d1_1.h>
#include <d2d1_1helper.h>
#include <d2d1effects.h>
#include <d3d11_1.h>
#include <dwrite_3.h>
#pragma comment(lib , "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib,"d3d11.lib")

#include <wincodecsdk.h>
#pragma comment(lib,"windowscodecs.lib")
#pragma endregion

/* 사용자 정의 헤더파일 */
#include "GameData.h"
#include "CommonMacroFunction.h"
#include "RandomFunction.h"
#include "KeyManager.h"
#include "ImageManager.h"
#include "TextManager.h"
#include "SoundManager.h"
#include "Utils.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"
#include "JsonSaveLoader.h"

#include "Camera.h"
#include "enumList.h"

using namespace MY_UTIL;

// # 싱 글 톤 #
#define RND RandomFunction::getSingleton()
#define KEYMANAGER KeyManager::getSingleton()
#define IMAGEMANAGER ImageManager::getSingleton()
#define TEXTMANAGER TextManager::getSingleton()
#define SOUNDMANAGER SoundManager::getSingleton()
#define TIMEMANAGER TimeManager ::getSingleton() // 타임매니저
#define SCENEMANAGER SceneManager ::getSingleton()
#define OBJECTMANAGER ObjectManager::getSingleton()
#define GAMEMANAGER GameData::getSingleton()
#define JSONDATAMANAGER JsonSaveLoader::getSingleton()


// # 매크로 # 윈도우 창 초기화
//전체화면
//#define FULL_SCREEN

#ifdef FULL_SCREEN
#define WIN_NAME	(LPSTR)(TEXT("WindowsAPI"))
#define WINSTART_X	0
#define WINSTART_Y	0
//GetSystemMetrics : 인자로 전달되는 시스템설정정보를 반환한다.
	//SM_CXSCREEN/ SM_CYSCREEN현재 화면 기준 해상도 x / y축 반환
#define WINSIZE_X	GetSystemMetrics(SM_CXSCREEN)
#define WINSIZE_Y	GetSystemMetrics(SM_CYSCREEN)
//					팝업 윈도우 생성 | 윈도우사이즈 최대화
#define WINSTYLE	WS_POPUPWINDOW | WS_MAXIMIZE
#else
#define WIN_NAME	(LPSTR)(TEXT("Titan Souls"))
#define WINSTART_X	300
#define WINSTART_Y	0
#define WINSIZE_X	1280
#define WINSIZE_Y	800

// WS_CAPTION 타이틀바를 가지기 위한 옵션
// WS_SYSMENU 제목표시줄에 컨트롤메뉴상자창을 만드는 옵션( _ ㅁ X )
#define WINSTYLE	WS_CAPTION | WS_SYSMENU
#endif // 0


// # 매크로 함수 #
#define SAFE_DELETE(p)	{if(p){delete(p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p)	{if(p){delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)		{if(p){(p)->Release();(p)=nullptr;}}


// # 전역 변수 #
#define DELTA_TIME		TIMEMANAGER->getElapsedTime()
#define PLAYER_MOVESPEED	OBJECTMANAGER->GetPlayer()->GetMoveSpeed()
#define PLAYER_DASHSPEED	OBJECTMANAGER->GetPlayer()->GetDashSpeed()

#define TILE_SIZE 16
#define IMAGE_SCALE 2
extern HINSTANCE		_hinstance;
extern HWND				_hWnd;
extern POINT			_ptMouse;

#define PLAYER			OBJECTMANAGER->GetPlayer()
#define CAMERA			SCENEMANAGER->GetCamera()

extern ID2D1Factory1*	_pD2Dfactory;
extern IDWriteFactory3*	_DWriteFactory;

//윈도우 코어 추가해보기..?
