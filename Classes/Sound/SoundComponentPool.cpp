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
	m_sounds.clear();
}

void SoundComponentPool::create(const char* path, bool loop, bool persistent)
{
	if(m_firstAvailable == NULL)
	{
		return;
	}
	SoundComponent* newSound = m_firstAvailable;
	m_firstAvailable = newSound->getNext();
	newSound->setNext(NULL);

	newSound->loadSound(path, loop, persistent);
	newSound->setPlay(true);

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
// 	for (int i = 0; i < m_poolSize - 1; i++)
// 	{
// 		if (!m_sounds[i]->process(dt) && !m_sounds[i]->isPersistent())
// 		{
// 			release(m_sounds[i]);
// 		}
// 	}
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