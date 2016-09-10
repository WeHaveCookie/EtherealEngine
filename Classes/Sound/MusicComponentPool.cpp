#include "stdafx.h"
#include "MusicComponentPool.h"

MusicComponentPool::MusicComponentPool(int size)
	:m_poolSize(size)
{
	m_music.reserve(size);
	for (int i = 0; i < size; i++)
	{
		m_music.push_back(new MusicComponent());
	}

	m_firstAvailable = m_music[0];

	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_music[i]->setNext(m_music[i + 1]);
	}

	m_music[m_poolSize - 1]->setNext(NULL);
}

MusicComponentPool::~MusicComponentPool()
{
	m_music.clear();
}

void MusicComponentPool::create(const char* path, bool loop, bool persistent)
{
	if (m_firstAvailable == NULL)
	{
		return;
	}
	MusicComponent* newMusic = m_firstAvailable;
	m_firstAvailable = newMusic->getNext();
	newMusic->setNext(NULL);

	newMusic->loadMusic(path, loop, persistent);
	newMusic->setPlay(true);
}

void MusicComponentPool::process(const float dt)
{
	for (int i = 0; i < m_poolSize - 1; i++)
	{
		if (!m_music[i]->process(dt) && !m_music[i]->isPersistent())
		{
			m_music[i]->setNext(m_firstAvailable);
			m_firstAvailable = m_music[i];
		}
	}
}

std::vector<MusicComponent*> MusicComponentPool::getMusicsUsed()
{
	std::vector<MusicComponent*> res;
	for (auto& music : m_music)
	{
		if (music->isUsed())
		{
			res.push_back(music);
		}
	}
	return res;
}