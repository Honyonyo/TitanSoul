#include "Stdafx.h"
#include "Timer.h"

HRESULT Timer::Init(void)
{
	//QueryPerformanceFrequency(): 1초당 진동수를 체크하며
	//고성능 타이머를 지원하면 t/ 아니라면 f 반환
	if (QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
	{
		_isHardware = true;
		// QueryPerformanceCounter(): 특정시점에 몆번 진동을 했는지
		// 체크하는 함수
		QueryPerformanceCounter((LARGE_INTEGER*)&_lastTime);
		// 초당 시간을 계산할 수 있는 시간 주기
		_timeScale = 1.0f / _periodFrequency;
	}
	// 고성능 타이머 지원을 안한다면
	else
	{
		_isHardware = false;
		//timeGetTime(): 보통 멀티미디어 타이머라고 불린다.
		// 운영체제가 시작되고 현재까지 흐른 시간을 밀리 세컨 단위로 반환
		// ㄴ 타이머는 운영체제가 시작되고 <- 가 안좋음 단일 객체로 하는게 좋음 운영체제에 종속되는것은 좋지 않음
		// 하지만 타임겟타임이 정밀도가 높기때문에 예외처리로 사용함(다만 요새 하드웨어가 고성능타이머 지원x 할일은 거의없음)
		_lastTime = timeGetTime();
		_timeScale = 0.001f;
	}

	_frameRate = 0;
	_FPSFrameCount = 0;
	_FPSTimeElapsed = 0;
	_worldTime = 0.0f;

	return S_OK;
}

// 현재 시간 계산
// 일단 60으로 잠궈놓지만 정 프레임이 떨어져서 
// 영상찍을때는 프레임을 풀고 영상찎는걸 추천함!
void Timer::tick(float lockFPS)
{
	if (_isHardware)
	{
		QueryPerformanceCounter((_LARGE_INTEGER*)&_curTime);
	}
	else
	{
		_curTime = timeGetTime();
	}

	// 마지막 시간과 현재시간의 경과량 측정
	_timeElapsed = (_curTime - _lastTime) * _timeScale;

	if (lockFPS > 0.0f)
	{
		while (_timeElapsed < (1.0f / lockFPS))
		{
			if (_isHardware) QueryPerformanceCounter((LARGE_INTEGER*)&_curTime);
			else _curTime = timeGetTime();

			_timeElapsed = (_curTime - _lastTime) * _timeScale;

		}
	}

	_lastTime = _curTime;
	_FPSFrameCount++;
	_FPSTimeElapsed += _timeElapsed;
	_worldTime += _timeElapsed;

	if (_FPSTimeElapsed > 1.0f)
	{
		_frameRate = _FPSFrameCount;
		_FPSFrameCount = 0;
		_FPSTimeElapsed = 0.0f;
	}
}

unsigned long Timer::getFrameRate(char* str) const
{
	if (str != nullptr)
	{
		wsprintf(str, "FPS:%d", _frameRate);
	}
	return _frameRate;
}
