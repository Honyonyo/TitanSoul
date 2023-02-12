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

	int   _frameNum;		//�����Ӽ�
	int   _frameWidth;		//�����Ӱ���ũ��
	int   _frameHeight;		//�����Ӽ���ũ��
	int   _frameNumWidth;	//�����������߰���
	int   _frameNumHeight;	//�����������߰���

	float _frameUpdateSec;	//�ʴ� ������ ������Ʈ ��
	double _elapsedSec;		//�����Ӱ� ������ ������ �ð�

	bool  _isPlay;			//�÷������̴�?
	bool  _loop;			//�ݺ�����?
	DWORD _nowPlayIdx;		//���� �÷��� �ε���

public:
	HRESULT Init(CImage* img);
	void Release(void);
	//��ü �����, �ݺ���� (������� �ܼ�������� �ƴ϶� �θ޶��� ����)
	void SetDefPlayFrame(bool reverse = false, bool loop = false);
	//���ϴ� �����Ӹ� �� ����ϱ� ���� �������ε����迭���޴´�
	void SetPlayFrame(int* playArr = nullptr, int arrLen = 0, bool loop = true);
	void SetPlayFrame(vector<int> playArr, bool loop = false, int startIdx = 0);

	//���������Ӱ� �������ӱ����� �����ݺ����
	void SetPlayFrame(int start, int end, bool reverse = false, bool loop = true);

	void SetFPS(int framePerSec);
	void FrameUpdate(float elapsedTime);

	void AniStart(void);
	void AniStop(void);
	void AniPause(void);
	void AniResume(void);

	//�÷������ΰ�?
	inline bool IsPlay(void) { return _isPlay; }
	//������ ��ġ get
	inline POINT GetFramePos(void) { return _frameList[_playList[_nowPlayIdx]]; }
	inline POINT GetFramePos(int num) { return _frameList[_playList[num]]; }
	//���� �ִϸ��̼��� ������ ������ ���´�
	inline int GetFrameIdx(void) {
		POINT ptPos = GetFramePos(_nowPlayIdx);
		int frameX = ptPos.x / _frameWidth;
		int frameY = ptPos.y / _frameHeight;

		return frameX + frameY * _frameNumWidth;
	}

	//������ ����ũ��
	inline int GetFrameWidth(void) { return _frameWidth; }
	//������ ����ũ��
	inline int GetFrameHeight(void) { return _frameHeight; }
	//������ ���ΰ���
	inline int GetFrameNumWidth(void) { return _frameNumWidth; }
	//������ ���ΰ���
	inline int GetFrameNumHeight(void) { return _frameNumHeight; }

	//���� ������ ���� �� ��°
	inline DWORD GetNowFrameIdx(void) { return _nowPlayIdx; }

	Animation(void);
	~Animation() {};
};