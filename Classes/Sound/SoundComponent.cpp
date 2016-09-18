#include "stdafx.h"
#include "SoundComponent.h"


uint32_t SoundComponent::newUID = 0;

SoundComponent::SoundComponent()
	:m_soundBuffers(),
	m_uid(newUID++)
{
	m_persistent = false;
	m_play = false;
	m_used = false;
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
	m_state.m_live.m_name = path;
	m_used = true;
}

bool SoundComponent::process(const float dt)
{
	if (m_play)
	{
		m_state.m_live.m_sound.play();
		m_play = false;
	}

	if (m_displayInfo)
	{
		displayInfo();
	}
	return !(m_state.m_live.m_sound.getStatus() == sf::SoundSource::Status::Stopped);
}

void SoundComponent::displayInfo()
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
				m_state.m_live.m_sound.pause();
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				stop();
			}
			ImGui::SameLine();
			bool loop = m_state.m_live.m_sound.getLoop();
			ImGui::Checkbox("Loop", &loop);
			m_state.m_live.m_sound.setLoop(loop);

			ImGui::Text("Persistent : %s", (m_persistent) ? "True" : "False");
			std::string status;
			switch (m_state.m_live.m_sound.getStatus())
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

			int volume = m_state.m_live.m_sound.getVolume();
			ImGui::SliderInt("Volume", &volume, 0, 100);
			m_state.m_live.m_sound.setVolume(volume);

			float pitch = m_state.m_live.m_sound.getPitch();
			ImGui::DragFloat("Pitch", &pitch, 0.01f, 0.0f, 10.0f);
			m_state.m_live.m_sound.setPitch(pitch);

			float currentMs = m_state.m_live.m_sound.getPlayingOffset().asMicroseconds() / 1000.0f;
			float oldOffset = currentMs;
			ImGui::SliderFloat("Duration", &currentMs, 1.0f, m_state.m_live.m_sound.getBuffer()->getDuration().asMicroseconds() / 1000.0f);

			if (oldOffset != currentMs)
			{
				m_state.m_live.m_sound.setPlayingOffset(sf::milliseconds(currentMs));
			}
		}
		ImGui::End();
	}
}