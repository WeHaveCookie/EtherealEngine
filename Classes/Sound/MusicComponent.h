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
	void setNext(MusicComponent* sound) { m_state.m_next = sound; }
	MusicComponent* getNext() { return m_state.m_next; }

private:
	bool				m_persistent;
	sf::Music			m_music;
	bool				m_play;

	union State
	{
		struct Live {

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