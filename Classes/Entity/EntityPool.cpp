#include "stdafx.h"
#include "EntityPool.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Thread/LoadingThread.h"
#include "Utils/containerUtils.h"

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
	for (auto& entity : m_entitys)
	{
		delete entity;
	}
	m_entitys.clear();
}

Entity* EntityPool::getNextEntity()
{
	if (m_firstAvailable == NULL)
	{
		return NULL;
	}
	Entity* newEntity = m_firstAvailable;
	m_firstAvailable = newEntity->getNext();
	m_usedEntity++;
	return newEntity;
}

Entity* EntityPool::create(const char* path)
{
	if (m_firstAvailable == NULL)
	{
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
		if (entity->isEdition())
		{
			auto currentMousePosition = InputMgr::getSingleton()->getMousePosition();
			auto entityBound = entity->getGlobalBounds();
			entity->addMotion(currentMousePosition - Vector2(entityBound.left + (entityBound.width / 2.0f),
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

	auto usedEntity = getUsedEntitysSortedLTH();
	for (auto& entity : usedEntity)
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

Entity* EntityPool::getEntity(std::string name)
{
	for (auto& entity : m_entitys)
	{
		if (strcmp(entity->getName(),name.c_str()) == 0 && entity->isAlive())
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
		if (entity->getUID() == id && entity->isAlive())
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


bool cmpDisplayLevelLTH(Entity* a, Entity* b)
{
	return a->getBackgroundLevel() >= b->getBackgroundLevel();

}

const std::vector<Entity*> EntityPool::getUsedEntitysSortedLTH() const 
{
	std::vector<Entity*> res;
	for (auto& entity : m_entitys)
	{
		if (entity->isAlive())
		{
			pushSorted(&res, entity, cmpDisplayLevelLTH);
		}
	}
	return res;
}

bool cmpDisplayLevelHTL(Entity* a, Entity* b)
{
	return a->getBackgroundLevel() < b->getBackgroundLevel();

}

const std::vector<Entity*> EntityPool::getUsedEntitysSortedHTL() const
{
	std::vector<Entity*> res;
	for (auto& entity : m_entitys)
	{
		if (entity->isAlive())
		{
			pushSorted(&res, entity, cmpDisplayLevelHTL);
		}
	}
	return res;
}
