#pragma once
#include "SingletonBase.h"

/*
 FMOD
 - sound engine lib
 - 언리얼과 유니티에는 라이센스 계약으로 기본 내장되어있다
	이외 대부분의 플랫폼에서는 사용하기 위한 선행처리가 필요하다.
		(CoCos2dx)
*/

/*
1. 사운드매니저 함수 추가
 - stl 1개 이상 사용
 추가할 함수 : 정지, 일시정지, 재개
 엔터 - 재생, p - 일시정지, 스페이스 - 정지
 편의성 고려하여 작성

 2. MP3플레이어 만들기 - 7곡
 - 재생버튼을 누르면 재생이 된다.
 - 재생을 누르면 버튼이 일시정지로 바뀐다.
 - 일시정지를 누르면 일시정지 된다. 버튼 재생으로 바뀐다.
 - 재생바 클릭시 중간부터 재생
 - 곡 재생시간을 표기한다. (현재 / 전체)
 - 한 곡의 재생이 끝나면 자동으로 다음 곡으로 넘어간다.

 정지버튼 재생버튼 이전곡버튼 다음곡버튼 재생바 곡재생시간
 볼륨조절필요
 앨범이미지 삽입

*/

enum class SOUNDKIND : UINT8
{
	SOUND_BGM = 0,
	SOUND_EFFECT_1,
	SOUND_EFFECT_2,
	SOUND_END
};


#define EXTRA_SOUND_CHANNEL 5		//여분의 채널 개수
#define TOTAL_SOUND_CHANNEL (static_cast<int>(SOUNDKIND::SOUND_END)) + EXTRA_SOUND_CHANNEL		//총 사운드 개수

using namespace FMOD;

struct scGroup
{
	Sound* sound;
	Channel* channel;

	string title;
	string filePath;
	string singer;
	string album;

	unsigned int totalTime;
};

class SoundManager : public SingletonBase <SoundManager>
{
private:
	System* _system;	//켜기끄기
	Sound** _sound;		//구체적인 실행제어
	Channel** _channel;	//서라운드채널, 메모리버퍼

	map<string, Sound*> _mapSound;
	map<string, Channel*> _mapChannel;
	map<string, scGroup*> _mapSC;

public:
	HRESULT Init(void);
	void Release(void);
	void Update(void);

	void setUp(string key, string fileName, string singer, string album, bool backGround, bool loop);
	void play(string key, float volume);
	bool pauseAndResume(string key);
	bool pauseAndResume(string key, bool paused);
	void stop(string key);

	void SetVolume(string key, float volume)
	{
		findSound(key)->channel->setVolume(volume);
	}
	void SetPosion(string key, int position)
	{
		scGroup* sound = nullptr;
		sound = findSound(key);
		if (sound != nullptr)
		{
			sound->channel->setPosition(position, FMOD_TIMEUNIT_MS);
		}
	}
	void SetPosition(string key, unsigned int position)
	{
		scGroup* sound = nullptr;
		sound = findSound(key);
		if (sound != nullptr)
		{
			sound->channel->setPosition(position, FMOD_TIMEUNIT_MS);
		}
	}

	unsigned int GetPosition(string key)
	{
		unsigned int position = 0;
		scGroup* sound = nullptr;
		sound = findSound(key);
		if (sound != nullptr)
		{
			sound->channel->getPosition(&position, FMOD_TIMEUNIT_MS);
		}
		return position;
	}

	void SetPlaylist(vector<string>* list)
	{
		map<string, scGroup*>::iterator iter = _mapSC.begin();
		for (iter; iter != _mapSC.end(); iter++)
		{
			list->push_back(iter->first);
		}
		cout<<_mapSC.size()<<endl;
		cout<<list->size()<<endl;
	}
	
	scGroup* findSound(string key)
	{
		auto findkey = _mapSC.find(key);

		if (findkey != _mapSC.end())
		{
			return findkey->second;
		}
		else
		{
			return nullptr;
			cout << "해당하는 노래가 없다. key : " << key << endl;
		}
	}

	SoundManager();
};

