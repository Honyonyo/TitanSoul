#pragma once
#include "SingletonBase.h"

//펑션키 고려해서 넉넉히 잡아준 키갯수
#define KEY_MAX 256

class KeyManager :public SingletonBase <KeyManager>
{
private:
	//BOOL _keyUp[KEY_MAX];
	//BOOL _keyDown[KEY_MAX];
	//true false가 싱글톤에서 머시기머시기 영향을 끼칠 수있기 때문에 비트셋으로 비트를 쪼갠 머시기머시기를 사용하기로 하였다.

	bitset<KEY_MAX> _keyUp;
	bitset<KEY_MAX> _keyDown;

public:
	HRESULT Init(void);

	//키가 한번 눌렸는지
	bool isOnceKeyDown(int key);
	//한번 누르고 띠었는지
	bool isOnceKeyUp(int key);
	//키가 계속 눌려있는지
	bool isStayKeyDown(int key);
	//토글키 - 누르면 상태가 유지되는
	bool isToggleKey(int key);

	bitset<KEY_MAX> getKeyUp(void) { return _keyUp; }
	bitset<KEY_MAX> getKeyDown(void) { return _keyDown; }

	void setKeyDown(int key, bool state) { _keyDown.set(key, state); }
	void setKeyUp(int key, bool state) { _keyUp.set(key, state); }

	KeyManager();
	~KeyManager();
};

