#pragma once
#include "Entity/Entity.h"

class EntityPool
{
public:

	EntityPool(int size);
	~EntityPool();
	Entity* create(const char* path);
	void process(const float dt);
	void paint();
	Entity* getEntity(uint32_t id);
	void release(uint32_t id);

	int getUsedEntity() { return m_usedEntity; }

private:
	void release(Entity* ent);

	const int				m_poolSize;
	int						m_usedEntity;
	std::vector<Entity*>	m_entitys;
	Entity*					m_firstAvailable;
};