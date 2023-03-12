#pragma once
//PreCompiledHeader PCH

//4005 : DXGI�� �׷��� �������̽����� ��ũ�� �����Ͽ� ���� ��µǴ� ���
//������Ʈ �Ӽ� ��޿��� ���� ����
#pragma warning(disable : 4005)
#define _CRT_SECURE_NO_WARNINGS

//���α׷����� ������ OS�� ���� Windows����̼� �ڵ忡 ���Ե� �Լ�, ������� �����ϴµ� ����� �ȴ�.
#include <SDKDDKVer.h>

//���� ������� �ʴ� ������ Windows������� ����
#define WIN32_LEAN_AND_MEAN
//GDI+�� ����Ϸ��� ������� �����;��Ѵ�.

//!Windows�������
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

//! �ܺ� �������
#include "FMOD\inc\FMOD\fmod.hpp"
#include "JsonCPP/include/json/json.h"

//!C ��Ÿ�� �������
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>
#include <mmsystem.h>

//!C++��Ÿ�� �������
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

#include <cassert>	//���� �������ּ���

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

/* ����� ���� ������� */
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

// # �� �� �� #
#define RND RandomFunction::getSingleton()
#define KEYMANAGER KeyManager::getSingleton()
#define IMAGEMANAGER ImageManager::getSingleton()
#define TEXTMANAGER TextManager::getSingleton()
#define SOUNDMANAGER SoundManager::getSingleton()
#define TIMEMANAGER TimeManager ::getSingleton() // Ÿ�ӸŴ���
#define SCENEMANAGER SceneManager ::getSingleton()
#define OBJECTMANAGER ObjectManager::getSingleton()
#define GAMEMANAGER GameData::getSingleton()
#define JSONDATAMANAGER JsonSaveLoader::getSingleton()


// # ��ũ�� # ������ â �ʱ�ȭ
//��üȭ��
//#define FULL_SCREEN

#ifdef FULL_SCREEN
#define WIN_NAME	(LPSTR)(TEXT("WindowsAPI"))
#define WINSTART_X	0
#define WINSTART_Y	0
//GetSystemMetrics : ���ڷ� ���޵Ǵ� �ý��ۼ��������� ��ȯ�Ѵ�.
	//SM_CXSCREEN/ SM_CYSCREEN���� ȭ�� ���� �ػ� x / y�� ��ȯ
#define WINSIZE_X	GetSystemMetrics(SM_CXSCREEN)
#define WINSIZE_Y	GetSystemMetrics(SM_CYSCREEN)
//					�˾� ������ ���� | ����������� �ִ�ȭ
#define WINSTYLE	WS_POPUPWINDOW | WS_MAXIMIZE
#else
#define WIN_NAME	(LPSTR)(TEXT("Titan Souls"))
#define WINSTART_X	300
#define WINSTART_Y	0
#define WINSIZE_X	1280
#define WINSIZE_Y	800

// WS_CAPTION Ÿ��Ʋ�ٸ� ������ ���� �ɼ�
// WS_SYSMENU ����ǥ���ٿ� ��Ʈ�Ѹ޴�����â�� ����� �ɼ�( _ �� X )
#define WINSTYLE	WS_CAPTION | WS_SYSMENU
#endif // 0


// # ��ũ�� �Լ� #
#define SAFE_DELETE(p)	{if(p){delete(p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p)	{if(p){delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)		{if(p){(p)->Release();(p)=nullptr;}}


// # ���� ���� #
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

//������ �ھ� �߰��غ���..?
