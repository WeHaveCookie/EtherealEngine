#pragma once
#include "Entity/Entity.h"

class EntityPool
{
public:

	EntityPool(int size);
	~EntityPool();
	void create(const char* path);
	void process(const float dt);
	void paint();

private:
	const int				m_poolSize;
	std::vector<Entity*>	m_entitys;
	Entity*					m_firstAvailable;
};