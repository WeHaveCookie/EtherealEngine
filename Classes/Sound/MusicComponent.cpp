#include "stdafx.h"
#include "MusicComponent.h"

uint32_t MusicComponent::newUID = 0;

MusicComponent::MusicComponent()
	:m_music(),
	m_uid(newUID++)
{
	m_persistent = false;
	m_play = false;
	m_used = false;
}

MusicComponent::~MusicComponent()
{

}

void MusicComponent::loadMusic(const char* path, bool loop, bool persistent)
{
	auto load = m_music.openFromFile(path);
	assert(load);
	m_music.setLoop(loop);
	m_persistent = persistent;
	m_state.m_live.m_name = path;
	m_used = true;
}

bool MusicComponent::process(const float dt)
{
	if (m_play)
	{
		m_music.play();
		m_play = false;
	}

	return !(m_music.getStatus() == sf::SoundSource::Status::Stopped);
}