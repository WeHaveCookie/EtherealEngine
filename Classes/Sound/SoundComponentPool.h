#pragma once
#include "SoundComponent.h"

class SoundComponentPool
{
public:

	SoundComponentPool(int size);
	~SoundComponentPool();
	void create(const char* path, bool loop, bool persistent);
	void process(const float dt);
	std::vector<SoundComponent*> getSoundsUsed();
	void release(uint32_t id);

private:
	void release(SoundComponent* sound);
	const int						m_poolSize;
	std::vector<SoundComponent*>	m_sounds;
	SoundComponent*					m_firstAvailable;
};