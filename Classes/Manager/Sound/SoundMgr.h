#pragma once

#include "Manager/Manager.h"
#include "Sound/MusicComponentPool.h"
#include "Sound/SoundComponentPool.h"

class SoundMgr : public Manager
{
	public:

		static SoundMgr* getSingleton() { return s_singleton; }

		SoundMgr();
		~SoundMgr();

		void init();
		void process(const float dt);
		void end();
		void showImGuiWindow(bool* window);

		uint32_t addSound(const char* path, bool loop = false, bool persistent = false);
		uint32_t addMusic(const char* path, bool loop = false, bool persistent = false);
		SoundComponent* getSound(uint32_t id);
		MusicComponent* getMusic(uint32_t id);
		void removeSound(uint32_t id);
		void removeMusic(uint32_t id);

		sf::Time getProcessTime() { return m_processTime; }

	private:

		static SoundMgr*		s_singleton;
		sf::Time				m_processTime;
		MusicComponentPool*		m_musics;
		SoundComponentPool*		m_sounds;
};