#pragma once

#include "Manager/Manager.h"
#include "Sound/MusicComponentPool.h"
#include "Sound/SoundComponentPool.h"

class SoundMgr : public Manager
{
	public:
		SoundMgr();
		~SoundMgr();

		void init();
		void process(const float dt);
		void end();
		void addSound(const char* path, bool loop = false, bool persistent = false);
		void addMusic(const char* path, bool loop = false, bool persistent = false);

	private:
		MusicComponentPool*		m_musics;
		SoundComponentPool*		m_sounds;
};