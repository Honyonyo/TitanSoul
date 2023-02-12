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
	// 어차피 이미지나 이런거 가따쓸필요가 없기 때문에 게임노드 상속x
	// 그러므로 코딩 규약만 맞춰서 써줌

	inline float getWorldTime(void) const { return _timer->getWorldTime(); }
	inline float getElapsedTime(void) const { return _timer->getElapsedTime(); }

	TimeManager() {}
	~TimeManager() {}
};

