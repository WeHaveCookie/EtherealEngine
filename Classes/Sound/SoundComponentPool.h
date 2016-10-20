#pragma once
#include "SoundComponent.h"

class SoundComponentPool
{
public:

	SoundComponentPool(int size);
	~SoundComponentPool();
	uint32_t create(const char* path, bool loop, bool persistent);
	SoundComponent* getSound(uint32_t id);

	void process(const float dt);
	std::vector<SoundComponent*> getSoundsUsed();
	void release(uint32_t id);

private:
	void release(SoundComponent* sound);
	const int						m_poolSize;
	std::vector<SoundComponent*>	m_sounds;
	SoundComponent*					m_firstAvailable;
};