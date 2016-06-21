#pragma once

enum MenuState {
	TITLE,
	CREDIT,
	OPTION,
	NOTHING
};

enum RessourcesType {
	NONE,
};

enum EventState {
	WAITED,
	TRIGGERED,
	ACCEPTED,
	FINISHED
};

enum ManagerType {
	MT_TIME,
	MT_SOUND,
	MT_LEVEL,
	MT_ITEM,
	MT_INPUT,
	MT_GAME,
	MT_EVENT,
	MT_ENGINE,
	MT_RENDER,
	MT_ENTITY
};

enum EntityAnimationState {
	EAS_RIGHT,
	EAS_LEFT,
	EAS_IDLE,
	EAS_IDLE_RIGHT,
	EAS_IDLE_LEFT,
};

struct EntityAnimation {
	EntityAnimationState	m_state;
	std::vector<sf::Sprite> m_animation;
	float					m_timePerFrame;
	int						m_currentFrame;
	bool					m_active;
};