#include "stdafx.h"
#include "soundManager.h"

soundManager::soundManager() : _system(NULL),
_channel(NULL),
_sound(NULL)
{
}

HRESULT soundManager::init(void)
{
	//���� �ý��� ����
	System_Create(&_system);

	//�ý��� �ʱ�ȭ
	_system->init(TOTAL_SOUND_CHANNEL, FMOD_INIT_NORMAL, 0);

	//ä�μ��� ���� ���� ���� �ִ°��� ����
	//ä�μ� ��ŭ �޸𸮹��� �� ���带 ����
	_sound = new Sound*[TOTAL_SOUND_CHANNEL];
	_channel = new Channel*[TOTAL_SOUND_CHANNEL];

	//�޸� �ѹ� ����
	memset(_sound, 0, sizeof(Sound*) * (TOTAL_SOUND_CHANNEL));
	memset(_channel, 0, sizeof(Channel*) * (TOTAL_SOUND_CHANNEL));

	this->addSound("Background", "Resources/mabinogi/sound/background/bg.mp3", true, true);
	this->addSound("InvenOpen", "Resources/mabinogi/sound/ui/inventory_open.wav", false, false);
	this->addSound("InvenClose", "Resources/mabinogi/sound/ui/inventory_close.wav", false, false);


	//�������
	this->addSound("WolfAttack", "Resources/mabinogi/sound/WolfSound/wolf01_natural_attack.wav", false, false);
	this->addSound("WolfCounter", "Resources/mabinogi/sound/WolfSound/wolf01_natural_attack_counter.wav", false, false);
	this->addSound("WolfSmash", "Resources/mabinogi/sound/WolfSound/wolf01_natural_attack_smash.wav", false, false);
	this->addSound("Wolfblow", "Resources/mabinogi/sound/WolfSound/wolf01_natural_blowaway.wav", false, false);
	this->addSound("WolfDown", "Resources/mabinogi/sound/WolfSound/wolf01_natural_down.wav", false, false);
	this->addSound("WolfHit", "Resources/mabinogi/sound/WolfSound/wolf01_natural_hit.wav", false, false);


	return S_OK;
}

void soundManager::release(void)
{
	//���� ����
	if (_channel != NULL || _sound != NULL)
	{
		for (int i = 0; i < TOTAL_SOUND_CHANNEL; i++)
		{
			if (_channel != NULL)
			{
				if (_channel[i]) _channel[i]->stop();
			}

			if (_sound != NULL)
			{
				if (_sound != NULL) _sound[i]->release();
			}
		}
	}

	SAFE_DELETE_ARRAY(_channel);
	SAFE_DELETE_ARRAY(_sound);

	//�ý��� ����
	if (_system != NULL)
	{
		_system->release();
		_system->close();
	}

	_mTotalSounds.clear();
}

//FMOD �ý��� ����
void soundManager::update(void)
{
	//���� �ý����� ��������� ������Ʈ
	//������ �ٲ�ų� ����� ���� ���带
	//ä�ο��� ���µ� �پ��� �۾��� �ڵ����� ���ش�
	_system->update();
}

void soundManager::addSound(string keyName, string soundName, bool background, bool loop)
{
	if (loop)
	{
		if (background)
		{
			//��������϶� -> createStream���� ����
			_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
		else
		{
			//�Ϲ� ����
			_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &_sound[_mTotalSounds.size()]);
		}
	}

	else
	{
		//�ѹ� �÷���
		_system->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &_sound[_mTotalSounds.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� �־��ش�
	_mTotalSounds.insert(make_pair(keyName, &_sound[_mTotalSounds.size()]));
}

void soundManager::play(string keyName, float volume)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_system->playSound(FMOD_CHANNEL_FREE, _sound[count], false, &_channel[count]);
			_channel[count]->setVolume(volume);
			break;
		}
	}
}

void soundManager::stop(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->stop();
			break;
		}
	}
}

void soundManager::pause(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(true);
			break;
		}
	}
}

void soundManager::resume(string keyName)
{
	arrSoundsIter iter = _mTotalSounds.begin();

	int count = 0;

	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->setPaused(false);
			break;
		}
	}
}

bool soundManager::isPlaySound(string keyName)
{
	bool isPlay;
	int count = 0;
	arrSoundsIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPlay);
			break;
		}
	}
	return isPlay;
}

bool soundManager::isPauseSound(string keyName)
{
	bool isPause;
	int count = 0;
	arrSoundsIter iter = _mTotalSounds.begin();
	for (iter; iter != _mTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			_channel[count]->isPlaying(&isPause);
			break;
		}
	}
	return isPause;
}
