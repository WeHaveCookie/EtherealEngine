#include "stdafx.h"
#include "EntityPool.h"
#include "Manager/Engine/PhysicMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Thread/LoadingThread.h"

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
	m_usedEntity = 0;
}

EntityPool::~EntityPool()
{
	m_entitys.clear();
}

Entity* EntityPool::getNextEntity()
{
	if (m_firstAvailable == NULL)
	{
		std::cout << "Cannot create new entity" << std::endl;
		return NULL;
	}
	Entity* newEntity = m_firstAvailable;
	m_firstAvailable = newEntity->getNext();
	m_usedEntity++;
	return newEntity;
}

Entity* EntityPool::create(const char* path)
{
	//assert(m_firstAvailable != NULL);
	if (m_firstAvailable == NULL)
	{
		std::cout << "Cannot create new entity" << std::endl;
		return NULL;
	}
	Entity* newEntity = m_firstAvailable;
	m_firstAvailable = newEntity->getNext();

	newEntity->build(path);
	m_usedEntity++;
	return newEntity;
}

void EntityPool::process(const float dt)
{
	for (auto& entity : m_entitys)
	{
		if (entity->isEditable())
		{
			auto currentMousePosition = InputMgr::getSingleton()->getMousePosition();
			auto entityBound = entity->getGlobalBounds();
			entity->addMotion(currentMousePosition - sf::Vector2f(entityBound.left + (entityBound.width / 2.0f),
				entityBound.top + (entityBound.height / 2.0f)));
		}

		if(!entity->process(dt))
		{
			release(entity);
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

void EntityPool::release(uint32_t id)
{
	for (auto& entity : m_entitys)
	{
		if (entity->getUID() == id)
		{
			release(entity);
			m_usedEntity--;
		}
	}
}

void EntityPool::release(Entity* ent)
{
	if (ent->isAlive())
	{
		ent->release();
		ent->setNext(m_firstAvailable);
		ent->closeInfo();
		m_firstAvailable = ent;
	}
}

const std::vector<Entity*> EntityPool::getUsedEntitys() const
{
	std::vector<Entity*> res;
	for (auto& entity : m_entitys)
	{
		if (entity->isAlive())
		{
			res.push_back(entity);
		}
	}
	return res;
}
