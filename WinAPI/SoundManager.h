#pragma once
#include "SingletonBase.h"

/*
 FMOD
 - sound engine lib
 - �𸮾�� ����Ƽ���� ���̼��� ������� �⺻ ����Ǿ��ִ�
	�̿� ��κ��� �÷��������� ����ϱ� ���� ����ó���� �ʿ��ϴ�.
		(CoCos2dx)
*/

/*
1. ����Ŵ��� �Լ� �߰�
 - stl 1�� �̻� ���
 �߰��� �Լ� : ����, �Ͻ�����, �簳
 ���� - ���, p - �Ͻ�����, �����̽� - ����
 ���Ǽ� ����Ͽ� �ۼ�

 2. MP3�÷��̾� ����� - 7��
 - �����ư�� ������ ����� �ȴ�.
 - ����� ������ ��ư�� �Ͻ������� �ٲ��.
 - �Ͻ������� ������ �Ͻ����� �ȴ�. ��ư ������� �ٲ��.
 - ����� Ŭ���� �߰����� ���
 - �� ����ð��� ǥ���Ѵ�. (���� / ��ü)
 - �� ���� ����� ������ �ڵ����� ���� ������ �Ѿ��.

 ������ư �����ư �������ư �������ư ����� ������ð�
 ���������ʿ�
 �ٹ��̹��� ����

*/

enum class SOUNDKIND : UINT8
{
	SOUND_BGM = 0,
	SOUND_EFFECT_1,
	SOUND_EFFECT_2,
	SOUND_END
};


#define EXTRA_SOUND_CHANNEL 5		//������ ä�� ����
#define TOTAL_SOUND_CHANNEL (static_cast<int>(SOUNDKIND::SOUND_END)) + EXTRA_SOUND_CHANNEL		//�� ���� ����

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
	System* _system;	//�ѱ����
	Sound** _sound;		//��ü���� ��������
	Channel** _channel;	//������ä��, �޸𸮹���

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
			cout << "�ش��ϴ� �뷡�� ����. key : " << key << endl;
		}
	}

	SoundManager();
};

