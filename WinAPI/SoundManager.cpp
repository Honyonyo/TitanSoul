#include "stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::Init(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//시스템 초기화
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);

	return S_OK;
}

void SoundManager::Release(void)
{
	//사운드 삭제
	auto iter = _mapSC.begin();
	for (; iter != _mapSC.end();) 
	{
		if (iter->second != nullptr) {
			if (iter->second->channel != nullptr)
				iter->second->channel->stop();
			if (iter->second->sound != nullptr)
			{
			//##	if (iter->second->sound != nullptr)
					//##iter->second->sound->Release();
			}
		}
		SAFE_DELETE(iter->second);
		iter = _mapSC.erase(iter);
	}

	if (_channel != nullptr || _sound != nullptr)
	{
		for (int i = 0; i < TOTAL_SOUND_CHANNEL; i++)
		{
			if (_channel != nullptr)
			{
				if (_channel[i]) _channel[i]->stop();
			}
			if (_sound != nullptr)
			{
				//##if (_sound != nullptr) _sound[i]->Release();
			}
		}
	}

	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	if (_system != nullptr)
	{
		//##_system->Release();
		_system->close();
	}
}

void SoundManager::Update(void)
{
	_system->update();
}

void SoundManager::setUp(string key, string fileName, string singer, string album, bool backGround, bool loop)
{
	scGroup* soundGroup;
	soundGroup = new scGroup;
	memset(&soundGroup->sound, 0, sizeof(Sound*));
	memset(&soundGroup->channel, 0, sizeof(Channel*));
	cout << "값넣기전"<<soundGroup->sound << endl;

	soundGroup->title= key;
	soundGroup->filePath = fileName;
	soundGroup->singer = singer;
	soundGroup->album = album;

	if (loop)
	{
		if (backGround)
		{
			_system->createStream(fileName.c_str(), FMOD_LOOP_NORMAL, 0, &soundGroup->sound);
		}//end background loop
		else
		{
			//동시에 여러음원 재생 가능
			_system->createSound(fileName.c_str(), FMOD_LOOP_NORMAL, 0, &soundGroup->sound);
		}//end effect loop
	}//end if loop
	else
	{
		_system->createSound(fileName.c_str(), DMDFO_DEFAULT, 0, &soundGroup->sound);
	}//end loop else

	soundGroup->sound->getLength(&soundGroup->totalTime, FMOD_TIMEUNIT_MS);

	_mapSC.insert(make_pair(key, soundGroup));
	cout << soundGroup->sound << endl;
}
	
//프로그래스바, 사운드, 이펙트, 애니메이션 계열
// 0.0~1.0
void SoundManager::play(string key, float volume)
{
	scGroup* sc = findSound(key);
	if (sc == nullptr)
	{
		return;
	}
	_system->playSound(FMOD_CHANNEL_FREE, (sc->sound), 0, &(sc->channel));
	sc->channel->setVolume(volume);
}

bool SoundManager::pauseAndResume(string key)
{
	scGroup* sc = findSound(key);
	if (sc == nullptr)
	{
		return false;
	}

	bool isPaused;
	sc->channel->getPaused(&isPaused);
	isPaused = !isPaused;
	sc->channel->setPaused(isPaused);

	return isPaused;
}

bool SoundManager::pauseAndResume(string key, bool paused)
{
	scGroup* sc = findSound(key);
	if (sc == nullptr)
	{
		return NULL;
	}

	sc->channel->setPaused(paused);
	return paused;
}

void SoundManager::stop(string key)
{
	scGroup* sc = findSound(key);
	if (sc == nullptr)
	{
		return;
	}

	sc->channel->stop();
}

SoundManager::SoundManager()
{}
