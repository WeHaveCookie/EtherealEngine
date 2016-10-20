#include "stdafx.h"
#include "SoundMgr.h"
#include "Manager/File/FileMgr.h"
#include "Utils/wcharUtils.h"

SoundMgr* SoundMgr::s_singleton = NULL;

SoundMgr::SoundMgr()
:Manager(ManagerType::Enum::Sound)
{
	s_singleton = this;
}

SoundMgr::~SoundMgr()
{
	delete m_musics;
	delete m_sounds;
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
		std::vector<std::wstring> files;
		FileMgr::GetFilesInDirectory(files, L"Data/Sound/Ambiant", L".ogg");
		char** musicsLabel = (char**)malloc(sizeof(char*) * files.size());
		static int musicID = 0;
		for (unsigned int i = 0; i < files.size(); i++)
		{
			musicsLabel[i] = (char*)malloc(sizeof(char) * files[i].size() + 1); // +1 for null terminated
			strcpy(musicsLabel[i], WideChartoAscii(files[i]).c_str());
		}

		ImGui::Combo("Music", &musicID, (const char**)musicsLabel, files.size());
		ImGui::SameLine();
		if (ImGui::Button("Create###1"))
		{
			SoundMgr::getSingleton()->addMusic(musicsLabel[musicID], false, true);
		}
		for (unsigned int i = 0; i < files.size(); i++)
		{
			free(musicsLabel[i]);
		}
		free(musicsLabel);
		

		{
			std::vector<std::wstring> files;
			FileMgr::GetFilesInDirectory(files, L"Data/Sound/FX", L".ogg");
			char** soundsLabel = (char**)malloc(sizeof(char*) * files.size());
			static int soundID = 0;

			for (unsigned int i = 0; i < files.size(); i++)
			{
				soundsLabel[i] = (char*)malloc(sizeof(char) * files[i].size() + 1); // +1 for null terminated
				strcpy(soundsLabel[i], WideChartoAscii(files[i]).c_str());
			}


			ImGui::Combo("Sound", &soundID, (const char**)soundsLabel, files.size());
			ImGui::SameLine();
			if (ImGui::Button("Create###2"))
			{
				SoundMgr::getSingleton()->addSound(soundsLabel[soundID], false, true);
			}
			for (unsigned int i = 0; i < files.size(); i++)
			{
				free(soundsLabel[i]);
			}
			free(soundsLabel);

		}

		if(ImGui::CollapsingHeader("Musics"))
		{

			for (auto& music : m_musics->getMusicsUsed())
			{
				ImGui::Text("%i : %s", music->getUID(), music->getName());
				if (ImGui::IsItemClicked())
				{
					music->showInfo();
				}
				ImGui::SameLine();
				std::string label = "Play###MP" + std::to_string(music->getUID());
				if (ImGui::Button(label.c_str()))
				{
					music->setPlay(true);
				}
				ImGui::SameLine();
				label = "Stop###MS" + std::to_string(music->getUID());
				if (ImGui::Button(label.c_str()))
				{
					music->stop();
				}

				ImGui::SameLine();
				bool loop = music->isLoop();
				label = "Loop###ML" + std::to_string(music->getUID());
				ImGui::Checkbox(label.c_str(), &loop);
				music->setLoop(loop);

				ImGui::SameLine();
				label = "Delete###MD" + std::to_string(music->getUID());
				if (ImGui::Button(label.c_str()))
				{
					removeMusic(music->getUID());
				}

			}

		}

		
		if (ImGui::CollapsingHeader("Sounds"))
		{

			for (auto& sound : m_sounds->getSoundsUsed())
			{
				ImGui::Text("%i : %s", sound->getUID(), sound->getName());
				if (ImGui::IsItemClicked())
				{
					sound->showInfo();
				}
				ImGui::SameLine();
				std::string label = "Play###SP" + std::to_string(sound->getUID());
				if (ImGui::Button(label.c_str()))
				{
					sound->setPlay(true);
				}
				ImGui::SameLine();
				label = "Stop###SS" + std::to_string(sound->getUID());
				if (ImGui::Button(label.c_str()))
				{
					sound->stop();
				}
				ImGui::SameLine();
				bool loop = sound->isLoop();
				label = "Loop###SL" + std::to_string(sound->getUID());
				ImGui::Checkbox(label.c_str(), &loop);
				sound->setLoop(loop);

				ImGui::SameLine();
				label = "Delete###SD" + std::to_string(sound->getUID());
				if (ImGui::Button(label.c_str()))
				{
					removeSound(sound->getUID());
				}
			}
		}
	}
	ImGui::End();
}

uint32_t SoundMgr::addSound(const char* path, bool loop, bool persistent)
{
	return m_sounds->create(path, loop, persistent);
}

uint32_t SoundMgr::addMusic(const char* path, bool loop, bool persistent)
{
	return m_musics->create(path, loop, persistent);
}

void SoundMgr::removeSound(uint32_t id)
{
	m_sounds->release(id);
}

void SoundMgr::removeMusic(uint32_t id)
{
	m_musics->release(id);
}

SoundComponent* SoundMgr::getSound(uint32_t id)
{
	return m_sounds->getSound(id);
}

MusicComponent* SoundMgr::getMusic(uint32_t id)
{
	return m_musics->getMusic(id);
}