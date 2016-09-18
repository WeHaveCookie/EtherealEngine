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
	m_displayInfo = false;
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

bool MusicComponent::process()
{
	if (m_play)
	{
		m_music.play();
		m_play = false;
	}

	if (m_displayInfo)
	{
		displayInfo();
	}

	return !(m_music.getStatus() == sf::SoundSource::Status::Stopped);
}

void MusicComponent::displayInfo()
{
	if (m_used)
	{
		std::string labelBegin = std::to_string(getUID()) + " : " + getName();
		if (ImGui::Begin(labelBegin.c_str(), &m_displayInfo, ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Button("Play"))
			{
				m_play = true;
			}
			ImGui::SameLine();
			if (ImGui::Button("Pause"))
			{
				m_music.pause();
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				stop();
			}
			ImGui::SameLine();
			bool loop = m_music.getLoop();
			ImGui::Checkbox("Loop", &loop);
			m_music.setLoop(loop);

			ImGui::Text("Persistent : %s", (m_persistent) ? "True" : "False");
			std::string status;
			switch (m_music.getStatus())
			{
			case sf::SoundSource::Status::Stopped:
				status = "Stopped";
				break;
			case sf::SoundSource::Status::Paused:
				status = "Paused";
				break;
			case sf::SoundSource::Status::Playing:
				status = "Playing";
				break;
			default:
				status = "unknown";
				break;
			}
			ImGui::Text("Status : %s", status.c_str());

			int volume = m_music.getVolume();
			ImGui::SliderInt("Volume", &volume, 0, 100);
			m_music.setVolume(volume);

			float pitch = m_music.getPitch();
			ImGui::DragFloat("Pitch", &pitch, 0.01f, 0.0f, 10.0f);
			m_music.setPitch(pitch);

			float currentMs = m_music.getPlayingOffset().asMicroseconds() / 1000000.0f;
			float oldOffset = currentMs;
			ImGui::SliderFloat("Duration", &currentMs, 1.0f, m_music.getDuration().asMicroseconds() / 1000000.0f);

			if (oldOffset != currentMs)
			{
				m_music.setPlayingOffset(sf::seconds(currentMs));
			}
		}
		ImGui::End();
	}
}