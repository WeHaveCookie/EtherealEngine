#pragma once
#include "MusicComponent.h"

class MusicComponentPool
{
public:

	MusicComponentPool(int size);
	~MusicComponentPool();
	uint32_t create(const char* path, bool loop, bool persistent);
	MusicComponent* getMusic(uint32_t id);
	void process(const float dt);
	std::vector<MusicComponent*> getMusicsUsed();
	void release(uint32_t id);

private:
	void release(MusicComponent* music);
	const int						m_poolSize;
	std::vector<MusicComponent*>	m_musics;
	MusicComponent*					m_firstAvailable;
};