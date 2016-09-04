#include "stdafx.h"
#include "MusicComponent.h"

MusicComponent::MusicComponent()
	:m_music()
{
	m_persistent = false;
	m_play = false;
}

MusicComponent::~MusicComponent()
{

}

void MusicComponent::loadMusic(const char* path, bool loop, bool persistent)
{
	assert(m_music.openFromFile(path));
	m_music.setLoop(loop);
	m_persistent = persistent;
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