#pragma once

class MusicComponent
{
public:
	MusicComponent();
	~MusicComponent();

	void loadMusic(const char* path, bool loop, bool persistent);
	bool process(const float dt);
	void setPlay(bool play) { m_play = play; }
	bool isPersistent() { return m_persistent; }
	void setNext(MusicComponent* sound) { m_used = false; m_state.m_next = sound; }
	MusicComponent* getNext() { return m_state.m_next; }
	uint32_t getUID() { return m_uid; }
	const char* getName() { return m_state.m_live.m_name.c_str(); }
	void stop() { m_music.stop(); }
	void used(bool b) { m_used = b; }
	bool isUsed() { return m_used; }
	void setLoop(bool b) { m_music.setLoop(b); }
	bool isLoop() { return m_music.getLoop(); }

protected:
	static uint32_t		newUID;
	const uint32_t		m_uid;

private:
	bool				m_persistent;
	sf::Music			m_music;
	bool				m_play;
	bool				m_used;

	union State
	{
		struct Live {
			std::string m_name;
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