#include "stdafx.h"
#include "SoundManager.h"

HRESULT SoundManager::Init(void)
{
	//사운드 시스템 생성
	System_Create(&_system);

	//시스템 초기화
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);

	SoundEffectLoad();
	BGMLoad();

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

	soundGroup->title = key;
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
	cout << key << ":" << soundGroup->sound << endl;
}

void SoundManager::setUp(string key, string fileName, bool backGround, bool loop)
{
	scGroup* soundGroup;
	soundGroup = new scGroup;
	memset(&soundGroup->sound, 0, sizeof(Sound*));
	memset(&soundGroup->channel, 0, sizeof(Channel*));

	soundGroup->title = key;
	soundGroup->filePath = fileName;

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
	cout << key << ":" << soundGroup->sound << endl;
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

void SoundManager::SoundEffectLoad()
{
	const int arrNumCount = 5;
	//캐릭터 이펙트 - 걷기
	for (int i = 0; i < arrNumCount; i++)
	{
		string tilespec[arrNumCount] = { "Snow","Vine","Water", "Stone", "Grass" };
		string path[arrNumCount];
		string key[arrNumCount];

		for (int j = 0; j < arrNumCount; j++)
		{
			path[j] = "Resources/Sound/Player/" + tilespec[j] + "/Step";
			key[j] = tilespec[j] + "Step";
		}

		for (int j = 1; j < 9; j++)
		{
			char num = j + 48;
			string Key = key[i] + num;
			string Path = path[i] + num + ".mp3";
			//cout << "key : " << Key << "\t";
			//cout << "path : " << Path << endl;

			setUp(Key, Path, false, false);
			play(Key, 1);
		}

	}

	//예티 사운드
	{
		string key = "Yeti_";
		string path = "Resources/Sound/Boss/YETI/";
		//예티 고드름
		for (int i = 1; i < 13; i++)
		{
			string KEY;
			string PATH;
			if (i < 10)
			{
				char num = i + 48;
				KEY = key + "IcicleLand" + num;
				PATH = path + "IcicleLand" + num + ".ogg";
			}
			else
			{
				char num1 = i / 10 + 48;
				char num2 = i % 10 + 48;
				KEY = key + "IcicleLand" + num1 + num2;
				PATH = path + "IcicleLand" + num1 + num2 + ".ogg";
			}
			//cout << KEY << "\t" << PATH << endl;
			setUp(KEY, PATH, false, false);
		}
		setUp("Yeti_IcicleRowLand", "Resources/Sound/Boss/YETI/IcicleRowLand.ogg", false, false);
		for (int i = 1; i < 11; i++)
		{
			string KEY;
			string PATH;
			if (i < 10)
			{
				char num = i + 48;
				KEY = key + "IcicleSmash" + num;
				PATH = path + "IcicleSmash" + num + ".ogg";
			}
			else
			{
				char num1 = i / 10 + 48;
				char num2 = i % 10 + 48;
				KEY = key + "IcicleSmash" + num1 + num2;
				PATH = path + "IcicleSmash" + num1 + num2 + ".ogg";
			}
			//cout << KEY << "\t" << PATH << endl;
			setUp(KEY, PATH, false, false);
		}//end icicle smash
		for (int i = 1; i < 5; i++)
		{
			string tmp[5] = { "Roll", "SnowImpact", "SnowThrow", "YetiBallImpact","YetiLand" };
			char num = i + 48;
			for (int j = 0; j < 5; j++)
			{
				string KEY = key + tmp[j] + num;
				string PATH = path + tmp[j] + num + ".ogg";
				setUp(KEY, PATH, false, false);
			}
		}//end roll, snowThrow, snowImpact,ballImpact, land
	}//end Yeti

	//콜로서스 사운드
	{
		string key = "Colossus_";
		string path = "Resources/Sound/Boss/COLOSSUS/";
		for (int i = 1; i < 5; i++)
		{
			char num = i + 48;
			string KEY = key + "Hit" + num;
			string PATH = path + "Hit" + num + ".ogg";
			setUp(KEY, PATH, false, false);
		}
		setUp(key + "Rise", path + "Rise.ogg", false, false);
		setUp(key + "RiseBodyLand", path + "RiseBodyLand.ogg", false, false);
		setUp(key + "Roar", path + "Roar.ogg", false, false);
		setUp(key + "Special", path + "Special.ogg", false, false);
		setUp(key + "SpecialBodyLand", path + "SpecialBodyLand.ogg", false, false);
		setUp(key + "SpecialFistLand", path + "SpecialFistLand.ogg", false, false);
	}


}

void SoundManager::BGMLoad()
{
	setUp("BGMColosus", "Resources/Sound/BGM/Colossus.mp3", true, true);
}

SoundManager::SoundManager()
{}
