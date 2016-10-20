#pragma once

class SoundComponent
{
public:
	SoundComponent();
	~SoundComponent();
	
	void loadSound(const char* path, bool loop, bool persistent);
	void setPlay(bool play) { m_play = play; }
	bool process(const float dt);
	const bool isPersistent() const { return m_persistent; }
	void setNext(SoundComponent* sound) { stop();  m_used = false; m_displayInfo = false; m_state.m_next = sound; }
	SoundComponent* getNext() { return m_state.m_next; }
	uint32_t getUID() { return m_uid; }
	const char* getName() const { return m_state.m_live.m_name.c_str(); }
	void play() { m_play = true; }
	void stop() { m_state.m_live.m_sound.stop(); }
	void setUsed(bool b) { m_used = b; }
	const bool isUsed() const { return m_used; }
	void setLoop(bool b) { m_state.m_live.m_sound.setLoop(b); }
	const bool isLoop() const { return m_state.m_live.m_sound.getLoop(); }
	void showInfo() { m_displayInfo = !m_displayInfo; }

protected:
	static uint32_t		newUID;
	const uint32_t		m_uid;

private:
	void displayInfo();
	bool m_persistent;
	sf::SoundBuffer		m_soundBuffers;
	bool				m_play;
	bool				m_used;
	bool				m_displayInfo;

	union State 
	{
		struct Live {
			sf::Sound			m_sound;
			std::string			m_name;
			Live()
				:m_sound()
			{
			}
		}m_live;

		SoundComponent* m_next;

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