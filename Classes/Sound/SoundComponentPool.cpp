#include "stdafx.h"
#include "SoundComponentPool.h"

SoundComponentPool::SoundComponentPool(int size)
	:m_poolSize(size)
{
	m_sounds.reserve(size);
	for (int i = 0; i < size; i++)
	{
		m_sounds.push_back(new SoundComponent());
	}

	m_firstAvailable = m_sounds[0];

	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_sounds[i]->setNext(m_sounds[i + 1]);
	}

	m_sounds[m_poolSize - 1]->setNext(NULL);
}

SoundComponentPool::~SoundComponentPool()
{
	for (auto& sound : m_sounds)
	{
		delete sound;
	}
	m_sounds.clear();
}

uint32_t SoundComponentPool::create(const char* path, bool loop, bool persistent)
{
	if(m_firstAvailable == NULL)
	{
		return -1;
	}
	SoundComponent* newSound = m_firstAvailable;
	m_firstAvailable = newSound->getNext();
	newSound->setNext(NULL);

	newSound->loadSound(path, loop, persistent);
	if (!persistent)
	{
		newSound->setPlay(true);
	}
	return newSound->getUID();

}

SoundComponent* SoundComponentPool::getSound(uint32_t id)
{
	for (auto& sound : m_sounds)
	{
		if (sound->getUID() == id)
		{
			return sound;
		}
	}
	return NULL;
}

void SoundComponentPool::process(const float dt)
{
	for (auto& sound : m_sounds)
	{
		if (!sound->process(dt) && !sound->isPersistent() && sound->isUsed())
		{
			release(sound);
		}
	}
}

std::vector<SoundComponent*> SoundComponentPool::getSoundsUsed()
{
	std::vector<SoundComponent*> res;
	for (auto& sound : m_sounds)
	{
		if (sound->isUsed())
		{
			res.push_back(sound);
		}
	}
	return res;
}

void SoundComponentPool::release(uint32_t id)
{
	for (auto& sound : m_sounds)
	{
		if (sound->getUID() == id)
		{
			release(sound);
		}
	}
}

void SoundComponentPool::release(SoundComponent* sound)
{
	sound->setNext(m_firstAvailable);
	m_firstAvailable = sound;
}