#pragma once

#pragma region 타이머
/*

	▶ 타이머

	- 기존에 사용한 API 공용함수(게임쪽으로는 효율이 아주 별로...)
	ㄴ GetTickCount();
	ㄴ GetTickCount64();
	- 시간을 측정하는 함수들은 기본적으로 윈도우가 시작된 이후 지난 시간을 밀리 세컨 단위로
	  반환한다.
	- 밀리 세컨드보다 좀 더 정밀한 타이머를 원한다면 고성능 타이머를 구현해야 한다.

	- 보통 타이머를 구현할때는 2가지를 기억하자
	ㄴ 1. 해상도
	ㄴ 2. 클럭(진동수)


	▷ 해상도란?
	- 다양한 시간 함수들은 시간을 측정하기 위한 Tick 계산 로직이 있다.
	  ㄴ 보통 1분을 기준으로 얼마나 많은 프레임으로 나눌 수 있는지 계산
	- 1분 동안 얼마나 많은 Tick으로 나눠서 시간의 정확도를 높이느냐가 고해상도와 저해상도의
	차이점이자 판단 기준

	▷ Clock

	- 타이머에서 얘기하는 클럭은 보통 CPU 클럭을 의미한다.
	- 클럭이란?
	ㄴ 클럭은 디지털 회로의 전기적 진동 수이며 Hz 단위로 표기
	ㄴ Ex: 1초에 1Hz면 신호를 한번 준다.(0또는 1)
	ㄴ 1GHz는 10에 9제곱 -> 10억번
	ㄴ 2.9ghz -> 1초에 29억번 비트를 옮긴다

	- 타이머 단위
	- Milli Second
	- 10 Milli Second
	- 100 Milli Second

	※ 고성능 타이머의 경우 중첩을 시키면 중첩을 시킨 만큼 효율이 올라간다.

*/
#pragma endregion

class Timer
{
private:
	float		_timeScale;   // 경과시간 조절
	float		_timeElapsed; // 마지막 시간과 현재시간의 경과값
	bool		_isHardware;   // 고성능 타이머 지원 여부

	__int64		_curTime; //  현재시간 __int 32 or 64(몆비트 사용하는지) 실행파일로 뽑아냈을때 64 ,32 갈림 __int64 32비트를 64로 강제로 바꿔준다 __ <- 표준어는 아니라는것
	__int64		_lastTime; // 이전시간
	__int64		_periodFrequency; // 시간 주기

	/*

	__int64 vs long

	- int vs lonng
	32비트에서 롱은 4비트 -> 64비트에서롱은 8비트
	*/

	unsigned long _frameRate;  //(초당프레임수)
	unsigned long   _FPSFrameCount; //fps 카운트
	float			_FPSTimeElapsed; //FPS 시간 경과량
	float			_worldTime;		 // 게임이 시작된 후 경과된 전체시간


public:
	HRESULT Init(void);
	void tick(float lockFPS = 60.0f);


	//현재 FPS
	unsigned long getFrameRate(char* str = nullptr) const; //?
	//마지막 시간과 현재 시간의 경과량
	inline float getElapsedTime(void) const { return _timeElapsed; } //?
	// 전체 경과 시간
	inline float getWorldTime(void) const { return _worldTime; } //?
	// 멤버함수의 끝에 컨스트가 뒤로 들어갔을때 -> 멤버함수의 상수화를 의미
	// ㄴ 이 함수내에서 멤버 변수의 값 변경을 막는다.
	// ㄴ 멤버변수가 변경될 수 있는 기회를 주지 않으며 변수 역시 변경 불가
	// ㄴ 포인터 x
	// 외부에서 값변경을 못하게 하기위해 막아줌

	// Timer -> Timer manger로 드갈것

	// 참고 -> 게임업계에서는 UPDATE잘 안씀 (프레임 드랍으로 인해 조건문들 통과하지 못하는경우도있음)
	// 위에 타이머로 뽑을경우 일정하게 뽑아낼수 있다


public:
	Timer() {};
	~Timer() {};
};

