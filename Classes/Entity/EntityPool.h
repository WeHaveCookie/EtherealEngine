#pragma once
#include "Entity/Entity.h"

class EntityPool
{
public:

	EntityPool(int size);
	~EntityPool();
	Entity* getNextEntity();
	Entity* create(const char* path);
	void process(const float dt);
	void paint();
	Entity* getEntity(uint32_t id);
	void release(uint32_t id);

	int getNumberUsedEntity() { return m_usedEntity; }
	std::vector<Entity*> getEntitys() { return m_entitys; }

private:
	void release(Entity* ent);

	const int				m_poolSize;
	int						m_usedEntity;
	std::vector<Entity*>	m_entitys;
	Entity*					m_firstAvailable;
};