#pragma once

class SoundComponent
{
public:
	SoundComponent();
	~SoundComponent();
	
	void loadSound(const char* path, bool loop, bool persistent);
	void setPlay(bool play) { m_play = play; }
	bool process(const float dt);
	bool isPersistent() { return m_persistent; }
	void setNext(SoundComponent* sound) { m_state.m_next = sound; }
	SoundComponent* getNext() { return m_state.m_next; }

private:
	bool m_persistent;
	sf::SoundBuffer		m_soundBuffers;
	bool				m_play;

	union State 
	{
		struct Live {
			sf::Sound			m_sound;
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