#include "stdafx.h"
#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::move(sf::Vector2f motion)
{

}

void Entity::paint()
{

}

void Entity::update()
{

}

void Entity::setId(const char* id)
{
	strcpy(m_id, id);
}

void Entity::addAnimation(EntityAnimation entAnim)
{
	m_animations.push_back(entAnim);
}

void Entity::setSpeed(float speed)
{
	m_speed = std::max(speed,0.0f);
}