#include "stdafx.h"
#include "SoundMgr.h"

SoundMgr::SoundMgr()
:Manager(ManagerType::Enum::Sound)
{

}

SoundMgr::~SoundMgr()
{

}

void SoundMgr::init()
{
	m_musics = new MusicComponentPool(10);
	m_sounds = new SoundComponentPool(244);
}

void SoundMgr::process(const float dt)
{
	m_sounds->process(dt);
	m_musics->process(dt);
}

void SoundMgr::end()
{
	free(m_sounds);
	free(m_musics);
}

void SoundMgr::addSound(const char* path, bool loop, bool persistent)
{
	m_sounds->create(path, loop, persistent);
}

void SoundMgr::addMusic(const char* path, bool loop, bool persistent)
{
	m_musics->create(path, loop, persistent);
}

