#include "stdafx.h"
#include "SoundComponent.h"

SoundComponent::SoundComponent()
	:m_soundBuffers()
{
	m_persistent = false;
	m_play = false;
}

SoundComponent::~SoundComponent()
{

}

void SoundComponent::loadSound(const char* path, bool loop, bool persistent)
{
	m_persistent = persistent;
	m_soundBuffers.loadFromFile(path);
	m_state.m_live.m_sound.setBuffer(m_soundBuffers);
	m_state.m_live.m_sound.setLoop(loop);
}

bool SoundComponent::process(const float dt)
{
	if (m_play)
	{
		m_state.m_live.m_sound.play();
		m_play = false;
	}
	auto test = m_state.m_live.m_sound.getStatus();
	return !(m_state.m_live.m_sound.getStatus() == sf::SoundSource::Status::Stopped);
}