#pragma once

#include "Graphics/DrawableObject.h"

struct EntityAnimation {
	std::vector<sf::Sprite> m_animation;
	float					m_timePerFrame;
	unsigned int			m_currentFrame;
	float					m_timeElapsedSinceUdpate = 0.0f;

	sf::Sprite* getCurrentAnimation() {
		if (m_timeElapsedSinceUdpate >= m_timePerFrame)
		{
			nextFrame();
			m_timeElapsedSinceUdpate = 0.0f;
		}
		return &m_animation[m_currentFrame];
	}

	void nextFrame() {
		m_currentFrame++;
		if (m_currentFrame >= m_animation.size())
		{
			m_currentFrame = 0;
		}
	}

	void update(const float dt)
	{
		m_timeElapsedSinceUdpate += dt;
	}

	void reset()
	{
		m_timeElapsedSinceUdpate = 0.0f;
		m_currentFrame = 0;
	}

};

namespace EntityAnimationState {
	enum Enum {
		Right,
		Left,
		Idle,
		IdleRight,
		IdleLeft
	};
}

class Entity : public DrawableObject
{
	public:
		Entity(const char* path);
		~Entity();

		virtual void move(sf::Vector2f motion);
		virtual void paint();
		virtual void update(const float dt);

		void setId(int id) { m_id = id; }
		void addAnimation(EntityAnimationState::Enum entAnimState, EntityAnimation entAnim);
		void setSpeed(float speed);
		const int getId() const { return m_id;}
		void setPosition(sf::Vector2f pos) { m_position = pos; }
		void setState(EntityAnimationState::Enum state);
		EntityAnimation* getAnimation(EntityAnimationState::Enum state);

	private:
		void build(const char* path);
		int														m_id;
		std::map<EntityAnimationState::Enum, EntityAnimation>	m_animations;
		float													m_speed;
		EntityAnimationState::Enum								m_currentState;
		sf::Vector2f											m_position;
		sf::Texture												m_texture;
};