#include "stdafx.h"
#include "MusicComponentPool.h"

MusicComponentPool::MusicComponentPool(int size)
	:m_poolSize(size)
{
	m_musics.reserve(size);
	for (int i = 0; i < size; i++)
	{
		m_musics.push_back(new MusicComponent());
	}

	m_firstAvailable = m_musics[0];

	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_musics[i]->setNext(m_musics[i + 1]);
	}

	m_musics[m_poolSize - 1]->setNext(NULL);
}

MusicComponentPool::~MusicComponentPool()
{
	for (auto& music : m_musics)
	{
		delete music;
	}
	m_musics.clear();
}

uint32_t MusicComponentPool::create(const char* path, bool loop, bool persistent)
{
	if (m_firstAvailable == NULL)
	{
		return -1;
	}
	MusicComponent* newMusic = m_firstAvailable;
	m_firstAvailable = newMusic->getNext();
	newMusic->setNext(NULL);

	newMusic->loadMusic(path, loop, persistent);
	if (!persistent)
	{
		newMusic->setPlay(true);
	}
	return newMusic->getUID();
}

MusicComponent* MusicComponentPool::getMusic(uint32_t id)
{
	for (auto& music : m_musics)
	{
		if (music->getUID() == id)
		{
			return music;
		}
	}
	return NULL;
}

void MusicComponentPool::process(const float dt)
{
	for (auto& music : m_musics)
	{
		if (!music->process() && !music->isPersistent() && music->isUsed())
		{
			release(music);
		}
	}
}

std::vector<MusicComponent*> MusicComponentPool::getMusicsUsed()
{
	std::vector<MusicComponent*> res;
	for (auto& music : m_musics)
	{
		if (music->isUsed())
		{
			res.push_back(music);
		}
	}
	return res;
}

void MusicComponentPool::release(uint32_t id)
{
	for (auto& music : m_musics)
	{
		if (music->getUID() == id)
		{
			release(music);
		}
	}
}

void MusicComponentPool::release(MusicComponent* music)
{
	music->setNext(m_firstAvailable);
	m_firstAvailable = music;
}
