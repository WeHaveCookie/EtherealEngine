#pragma once
#include "MusicComponent.h"

class MusicComponentPool
{
public:

	MusicComponentPool(int size);
	~MusicComponentPool();
	void create(const char* path, bool loop, bool persistent);
	void process(const float dt);
	std::vector<MusicComponent*> getMusicsUsed();

private:
	const int						m_poolSize;
	std::vector<MusicComponent*>	m_music;
	MusicComponent*					m_firstAvailable;
};