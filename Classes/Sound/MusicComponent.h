#pragma once

class MusicComponent
{
public:
	MusicComponent();
	~MusicComponent();

	void loadMusic(const char* path, bool loop, bool persistent);
	bool process();
	void setPlay(bool play) { m_play = play; }
	const bool isPersistent() const { return m_persistent; }
	void setNext(MusicComponent* sound) { stop(); m_displayInfo = false; m_used = false; m_state.m_next = sound; }
	MusicComponent* getNext() { return m_state.m_next; }
	uint32_t getUID() { return m_uid; }
	const char* getName() { return m_state.m_live.m_name.c_str(); }
	void stop() { m_music.stop(); }
	void play() { m_play = true; }
	void setUsed(bool b) { m_used = b; }
	const bool isUsed() const { return m_used; }
	void setLoop(bool b) { m_music.setLoop(b); }
	const bool isLoop() const{ return m_music.getLoop(); }
	const bool isPlayed() const;
	const float getVolume() const { return m_music.getVolume(); }
	void setVolume(const float vol) { m_music.setVolume(vol); }
	void showInfo() { m_displayInfo = !m_displayInfo; }
	void closeInfo() { m_displayInfo = false; }
	const uint32_t getLayerLevel() const { return m_state.m_live.m_layer; }
	void setLayer(const uint32_t layer) { m_state.m_live.m_layer = layer; }
	void mute();
	void unmute();

protected:
	static uint32_t		newUID;
	const uint32_t		m_uid;

private:
	void displayInfo();
	bool				m_persistent;
	sf::Music			m_music;
	bool				m_play;
	bool				m_used;
	bool				m_displayInfo;
	float				m_lastVolume;

	union State
	{
		struct Live {
			std::string m_name;
			uint32_t	m_layer;
			Live()
			{
			}
		}m_live;

		MusicComponent* m_next;

		State()
			:m_live()
		{
			m_next = NULL;
		}

		~State()
		{
		}

	}m_state;
};