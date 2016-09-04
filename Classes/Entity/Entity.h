#pragma once

#include "Graphics/DrawableObject.h"

#define ERROR_TEXTURE "Data/Texture/error.png"

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
		startEnum = Right,
		Left,
		Idle,
		IdleRight,
		IdleLeft,
		endEnum = IdleLeft
	};
}

class Entity : public DrawableObject
{
	public:
		Entity();
		~Entity();

		virtual void move(sf::Vector2f motion);
		virtual void paint();
		virtual void update(const float dt);
		bool process(const float dt);

		void setName(const char* name) { strcpy(m_state.m_live.m_name, name); }
		void addAnimation(EntityAnimationState::Enum entAnimState, EntityAnimation entAnim);
		void setSpeed(float speed);
		const char* getId() const { return m_state.m_live.m_name;}
		const unsigned int getUID() const { return m_uid; }
		void setPosition(sf::Vector2f pos) { m_position = pos; }
		void setState(EntityAnimationState::Enum state);
		EntityAnimation* getAnimation(EntityAnimationState::Enum state);
		void setNext(Entity* ent) { m_state.m_next = ent; }
		Entity* getNext() { return m_state.m_next; }

	protected:
		static unsigned int newUID;
		const unsigned int m_uid;

	private:
		friend class EntityPool;
		void build(const char* path);

		bool m_live;

		union State
		{
			struct
			{
				char													m_name[128];
				std::map<EntityAnimationState::Enum, EntityAnimation>	m_animations;
				float													m_speed;
				EntityAnimationState::Enum								m_currentState;
				sf::Vector2f											m_position;
				sf::Texture												m_texture;
				sf::Texture												m_errorTexture;
				unsigned int											m_height;
				unsigned int											m_width;

				void clear()
				{
					for (int i = EntityAnimationState::startEnum; i <= EntityAnimationState::endEnum; i++)
					{
						auto ste = static_cast<EntityAnimationState::Enum>(i);
						sf::Sprite spr;
						spr.setTexture(m_errorTexture);
						spr.setTextureRect(sf::IntRect(0 , 0, m_width, m_height));
						m_animations[ste].m_animation.push_back(spr);
					}
				}
			} m_live;

			Entity*		m_next;

			State()
				:m_live()
			{
				m_next = NULL;
			}

			~State()
			{
			}

		} m_state;
};