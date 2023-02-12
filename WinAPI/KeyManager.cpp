#include "stdafx.h"
#include "KeyManager.h"

HRESULT KeyManager::Init(void)
{
    //키 안눌린것으로 전부 초기화
    for (int i = 0; i < KEY_MAX; i++) {
        this->setKeyDown(i, false);
        this->setKeyUp(i, false);
    }
    return S_OK;

}

bool KeyManager::isOnceKeyDown(int key)
{
    /*
GetAsyncKeyState
 - 어떤 키가 눌렸는지 체크, 프로그램에서 키를 입력받고 싶을 때 사용하는 API공용 함수
 - 메세지 큐에 있는 키의 정보를 확인(가져)한다.
 - 키가 눌린 시점을 체크하기 위해 &(And)연산을 사용한다.
*/
//AND연산~ : "비트단위"로 and연산을 수행한다. 둘 다 1일때만 결과가 1이고, 아니라면 0이다.

    if (GetAsyncKeyState(key) & 0x8000) 
    {
        if (!this->getKeyDown()[key]) {
            this->setKeyDown(key, true);
            return true;
        }
    }
    else this->setKeyDown(key, false);
    return false;
}

bool KeyManager::isOnceKeyUp(int key)
{
    if (GetAsyncKeyState(key) & 0x8000) {
        this->setKeyUp(key, true);
    }
    else {
        if (this->getKeyUp()[key]) {
            this->setKeyUp(key, false);
            return true;
        }
    }
    return false;
}

bool KeyManager::isStayKeyDown(int key)
{
    if (GetAsyncKeyState(key) & 0x8000) return true;

    return false;
}

bool KeyManager::isToggleKey(int key)
{
    if (GetKeyState(key) & 0x0001) return true;

    return false;
}

KeyManager::KeyManager()
{
}

KeyManager::~KeyManager()
{
}
