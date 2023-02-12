#pragma once
#include "Arrow.h"
class Animation
{
public:
	typedef vector<POINT> _vFrameList;
	typedef vector<int> _vPlayList;

private:
	_vFrameList _frameList;
	_vPlayList _playList;

	int   _frameNum;		//프레임수
	int   _frameWidth;		//프레임가로크기
	int   _frameHeight;		//프레임세로크기
	int   _frameNumWidth;	//가로프레임중갯수
	int   _frameNumHeight;	//세로프레임중갯수

	float _frameUpdateSec;	//초당 프레임 업데이트 수
	double _elapsedSec;		//프레임과 프레임 사이의 시간

	bool  _isPlay;			//플레이중이니?
	bool  _loop;			//반복도니?
	DWORD _nowPlayIdx;		//현재 플레이 인덱스

public:
	HRESULT Init(CImage* img);
	void Release(void);
	//전체 역재생, 반복재생 (역재생이 단순역재생이 아니라 부메랑을 말함)
	void SetDefPlayFrame(bool reverse = false, bool loop = false);
	//원하는 프레임만 찍어서 재생하기 위한 프레임인덱스배열을받는다
	void SetPlayFrame(int* playArr = nullptr, int arrLen = 0, bool loop = true);
	void SetPlayFrame(vector<int> playArr, bool loop = false, int startIdx = 0);

	//시작프레임과 끝프레임까지를 구간반복재생
	void SetPlayFrame(int start, int end, bool reverse = false, bool loop = true);

	void SetFPS(int framePerSec);
	void FrameUpdate(float elapsedTime);

	void AniStart(void);
	void AniStop(void);
	void AniPause(void);
	void AniResume(void);

	//플레이중인가?
	inline bool IsPlay(void) { return _isPlay; }
	//프레임 위치 get
	inline POINT GetFramePos(void) { return _frameList[_playList[_nowPlayIdx]]; }
	inline POINT GetFramePos(int num) { return _frameList[_playList[num]]; }
	//현재 애니메이션의 프레임 순서를 얻어온다
	inline int GetFrameIdx(void) {
		POINT ptPos = GetFramePos(_nowPlayIdx);
		int frameX = ptPos.x / _frameWidth;
		int frameY = ptPos.y / _frameHeight;

		return frameX + frameY * _frameNumWidth;
	}

	//프레임 가로크기
	inline int GetFrameWidth(void) { return _frameWidth; }
	//프레임 세로크기
	inline int GetFrameHeight(void) { return _frameHeight; }
	//프레임 가로갯수
	inline int GetFrameNumWidth(void) { return _frameNumWidth; }
	//프레임 세로갯수
	inline int GetFrameNumHeight(void) { return _frameNumHeight; }

	//현재 프레임 순서 몇 번째
	inline DWORD GetNowFrameIdx(void) { return _nowPlayIdx; }

	Animation(void);
	~Animation() {};
};