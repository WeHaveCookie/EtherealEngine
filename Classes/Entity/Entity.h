#pragma once

#include "Graphics/DrawableObject.h"

class Entity : public DrawableObject
{
	public:
		Entity();
		~Entity();

		virtual void move(sf::Vector2f motion);
		virtual void paint();
		virtual void update();

		void setId(const char* id);
		void addAnimation(EntityAnimation entAnim);
		void setSpeed(float speed);
		const char* getId() const { return m_id;}
		

	private:
		char*								m_id;
		std::vector<EntityAnimation>		m_animations;
		float								m_speed;
		EntityAnimationState				m_currentState;
};