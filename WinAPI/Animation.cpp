#include "stdafx.h"
#include "Animation.h"

HRESULT Animation::Init(CImage* img)
{
    //���� ������ ��
    _frameWidth = img->GetFrameWidth();
    _frameNumWidth = img->GetWidth() / _frameWidth;
    //���� ������ ��
    _frameHeight = img->GetFrameHeight();
    _frameNumHeight = img->GetHeight() / _frameHeight;
    //�� ������ ��
    _frameNum = _frameNumWidth* _frameNumHeight;

    _frameList.clear();

    for (int i = 0; i < _frameNumHeight; i++) 
    {
        for (int j = 0; j < _frameNumWidth; j++)
        {
            POINT framePos;
            framePos.x = j * _frameWidth;
            framePos.y = i * _frameHeight;

            _frameList.push_back(framePos);
        }
    }
    SetDefPlayFrame();

    cout << "0��° ������ ���� "<<GetFramePos(0).x << ", " << GetFramePos(0).y << endl;

    return S_OK;
}

void Animation::Release(void)
{
    //Do nothing
}

//�⺻ ������ ����
void Animation::SetDefPlayFrame(bool reverse, bool loop)
{
    _loop = loop;
    _playList.clear();

    if (reverse)
    {
        if (_loop)
        {
            for (int i = 0; i < _frameNum; i++)
            {
                _playList.push_back(i);
            }

            for (int i = _frameNum - 2; i > 0; i--)
            {
                _playList.push_back(i);
            }
        }//end if loop
        else
        {
            for (int i = 0; i < _frameNum; i++)
            {
                _playList.push_back(i);
            }

            for (int i = _frameNum - 2; i >= 0; i--)
            {
                _playList.push_back(i);
            }
        }//end else loop
    }//end if reverse
    else
    {
        for (int i = 0; i < _frameNum; i++)
        {
            _playList.push_back(i);
        }
    }
}

//���ϴ� �����Ӹ� ���
void Animation::SetPlayFrame(int* playArr, int arrLen, bool loop)
{
    _loop = loop;    
    _playList.clear();

    if (_loop)
    {
        for (int i = 0; i < arrLen; i++)
        {
            _playList.push_back(playArr[i]);
        }
    }//end if loop
    else
    {
        for (int i = 0; i < arrLen; i++)
        {
            _playList.push_back(playArr[i]);
        }
    }//end else loop
}

void Animation::SetPlayFrame(vector<int> playArr, bool loop, int startIdx)
{
    _loop = loop;    
    _nowPlayIdx = startIdx;
    _playList.clear();
    _vPlayList empty;
    empty.swap(_playList);

    if (_loop)
    {
        for (int i = 0; i < playArr.size(); i++)
        {
            _playList.push_back(playArr[i]);
        }
    }//end if loop
    else
    {
        for (int i = 0; i < playArr.size(); i++)
        {
            _playList.push_back(playArr[i]);
        }
    }//end else loop
}

//������ �߶� ���
void Animation::SetPlayFrame(int start, int end, bool reverse, bool loop)
{
    _loop = loop;
    _playList.clear();

    int firstFrame = start >= 0 ? start : 0;
    int lastFrame = end < _frameNum ? end : _frameNum;
    if (reverse)
    {
        if (_loop)
        {
            for (int i = firstFrame; i < lastFrame; i++)
            {
                _playList.push_back(i);
            }

            for (int i = _frameNum - 2; i > 0; i--)
            {
                _playList.push_back(i);
            }
        }//end if loop
        else
        {
            for (int i = firstFrame; i < lastFrame; i++)
            {
                _playList.push_back(i);
            }

            for (int i = _frameNum - 2; i >= 0; i--)
            {
                _playList.push_back(i);
            }
        }//end else loop
    }//end if reverse
    else
    {
        for (int i = firstFrame; i < lastFrame; i++)
        {
            _playList.push_back(i);
        }
    }
}

void Animation::SetFPS(int framePerSec)
{
    _frameUpdateSec = 1.0f / framePerSec;
}

void Animation::FrameUpdate(float elapsedTime)
{
    if (_isPlay)
    {
        if (_elapsedSec < 0) _elapsedSec = 0;
        _elapsedSec += elapsedTime;
        //�����Ӿ�����Ʈ�ð��� �Ǿ�����
        if (_elapsedSec >= _frameUpdateSec)
        {
            _elapsedSec -= _frameUpdateSec;

            _nowPlayIdx++;
            if (_nowPlayIdx == _playList.size())
            {
                if (_loop)
                {
                    _nowPlayIdx = 0;
                }
                else
                {
                    _nowPlayIdx--;
                    _isPlay = false;
                }
            }//end if �ε�����������
        }//end �����Ӿ�����Ʈ�ð��� �Ǿ�����
    }
}

void Animation::AniStart(void)
{
    _isPlay = true;
    _nowPlayIdx = 0;
}

void Animation::AniStop(void)
{
    _isPlay = false;
    _nowPlayIdx = 0;
}

void Animation::AniPause(void)
{
    _isPlay = false;
}

void Animation::AniResume(void)
{
    _isPlay = true;
}

Animation::Animation(void) : _frameNum(0),
                             _frameWidth(0),
                             _frameHeight(0),
                             _frameNumWidth(0),
                             _frameNumHeight(0),
                             _frameUpdateSec(0),
                             _elapsedSec(0),
                             _isPlay(false),
                             _loop(false),
                             _nowPlayIdx(0)
{
}
