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
	Entity* getEntity(std::string name);
	void release(uint32_t id);

	const int getNumberUsedEntity() const { return m_usedEntity; }
	const std::vector<Entity*> getEntitys() const { return m_entitys; }
	const std::vector<Entity*> getUsedEntitys() const;
	const std::vector<Entity*> getUsedEntitysSortedLTH() const;
	const std::vector<Entity*> getUsedEntitysSortedHTL() const;

	const int getPoolSize() const { return m_poolSize; }

private:
	void release(Entity* ent);

	const int				m_poolSize;
	int						m_usedEntity;
	std::vector<Entity*>	m_entitys;
	Entity*					m_firstAvailable;
};