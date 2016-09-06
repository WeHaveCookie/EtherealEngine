#include "stdafx.h"
#include "EntityPool.h"
#include "Manager/Engine/PhysicMgr.h"


EntityPool::EntityPool(int size)
	:m_poolSize(size)
{
	m_entitys.reserve(size);
	for (int i = 0; i < size; i++)
	{
		m_entitys.push_back(new Entity());
	}

	m_firstAvailable = m_entitys[0];

	for (int i = 0; i < m_poolSize - 1; i++)
	{
		m_entitys[i]->setNext(m_entitys[i + 1]);
	}

	m_entitys[m_poolSize - 1]->setNext(NULL);
}

EntityPool::~EntityPool()
{
	m_entitys.clear();
}

void EntityPool::create(const char* path)
{
	assert(m_firstAvailable != NULL);

	Entity* newEntity = m_firstAvailable;
	m_firstAvailable = newEntity->getNext();

	newEntity->build(path);
	PhysicMgr::getSingleton()->registerEntity(newEntity);
}

void EntityPool::process(const float dt)
{
	for (auto& entity : m_entitys)
	{
		if(!entity->process(dt))
		{
			entity->setNext(m_firstAvailable);
			m_firstAvailable = entity;
			PhysicMgr::getSingleton()->unregisterEntity(entity);
		}
	}
}

void EntityPool::paint()
{
	for (auto& entity : m_entitys)
	{
		entity->paint();
	}
}

Entity* EntityPool::getEntity(uint32_t id)
{
	for (auto& entity : m_entitys)
	{
		if (entity->getUID() == id)
		{
			return entity;
		}
	}
	return NULL;
}
