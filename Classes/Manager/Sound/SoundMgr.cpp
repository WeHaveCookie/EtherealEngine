#include "stdafx.h"
#include "SoundMgr.h"

SoundMgr* SoundMgr::s_singleton = NULL;

SoundMgr::SoundMgr()
:Manager(ManagerType::Enum::Sound)
{
	s_singleton = this;
}

SoundMgr::~SoundMgr()
{

}

void SoundMgr::init()
{
	m_processTime = sf::Time::Zero;
	m_musics = new MusicComponentPool(10);
	m_sounds = new SoundComponentPool(244);
}

void SoundMgr::process(const float dt)
{
	sf::Clock clock;
	m_sounds->process(dt);
	m_musics->process(dt);
	m_processTime = clock.getElapsedTime();
}

void SoundMgr::end()
{
	free(m_sounds);
	free(m_musics);
}

void SoundMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("SoundMgr", window))
	{
		static char buffer[128];
		ImGui::InputText("Name", buffer, 128);
		ImGui::SameLine();
		std::string path;
		if (ImGui::Button("Music"))
		{
			path = "Data/Sound/Ambiant" + std::string(buffer) + ".ogg";
			SoundMgr::getSingleton()->addMusic(path.c_str(), false, true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Sound"))
		{
			path = "Data/Sound/FX/" + std::string(buffer) + ".ogg";
			SoundMgr::getSingleton()->addSound(path.c_str(), false, true);
		}

		if(ImGui::CollapsingHeader("Musics"))
		{
			for (auto& music : m_musics->getMusicsUsed())
			{
				ImGui::Text("%i : %s", music->getUID(), music->getName());
				ImGui::SameLine();
				if (ImGui::Button("Play"))
				{
					music->setPlay(true);
				}
				ImGui::SameLine();
				if (ImGui::Button("Stop"))
				{
					music->stop();
				}
				ImGui::SameLine();
				bool loop = music->isLoop();
				ImGui::Checkbox("Loop", &loop);
				music->setLoop(loop);
			}

		}

		if (ImGui::CollapsingHeader("Sounds"))
		{
			for (auto& sound : m_sounds->getSoundsUsed())
			{
				ImGui::Text("%i : %s", sound->getUID(), sound->getName());
				ImGui::SameLine();
				if (ImGui::Button("Play"))
				{
					sound->setPlay(true);
				}
				ImGui::SameLine();
				if (ImGui::Button("Stop"))
				{
					sound->stop();
				}
				ImGui::SameLine();
				bool loop = sound->isLoop();
				ImGui::Checkbox("Loop", &loop);
				sound->setLoop(loop);
			}
		}

		ImGui::End();
	}
}

void SoundMgr::addSound(const char* path, bool loop, bool persistent)
{
	m_sounds->create(path, loop, persistent);
}

void SoundMgr::addMusic(const char* path, bool loop, bool persistent)
{
	m_musics->create(path, loop, persistent);
}

