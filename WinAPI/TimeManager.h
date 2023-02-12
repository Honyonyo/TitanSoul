#pragma once
#include"SingletonBase.h"
#include"Timer.h"

class TimeManager : public SingletonBase<TimeManager>
{
private:
	Timer* _timer;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(float lock = 0.0f);
	void Render();
	// ������ �̹����� �̷��� �������ʿ䰡 ���� ������ ���ӳ�� ���x
	// �׷��Ƿ� �ڵ� �Ծุ ���缭 ����

	inline float getWorldTime(void) const { return _timer->getWorldTime(); }
	inline float getElapsedTime(void) const { return _timer->getElapsedTime(); }

	TimeManager() {}
	~TimeManager() {}
};

