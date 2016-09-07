#include "stdafx.h"
#include "EntityMgr.h"

#include "../../External/rapidjson/document.h"
#include "Entity/EntityPool.h"

EntityMgr* EntityMgr::s_singleton = NULL;

EntityMgr::EntityMgr()
:Manager(ManagerType::Enum::Entity)
{
	s_singleton = this;
	m_entitys = new EntityPool(100);
}

EntityMgr::~EntityMgr()
{

}

void EntityMgr::init()
{
}

void EntityMgr::process(const float dt)
{
	sf::Clock clock;
	m_entitys->process(dt);
	m_processTime = clock.getElapsedTime();
}

void EntityMgr::end()
{
	free(m_entitys);
}

void EntityMgr::paint()
{
	m_entitys->paint();
}

Entity* EntityMgr::buildEntity(const char* path)
{
	return m_entitys->create(path);
}

void EntityMgr::removeEntity(uint32_t id)
{
	m_entitys->release(id);
}

Entity* EntityMgr::getEntity(uint32_t id)
{
	return m_entitys->getEntity(id);
}

int EntityMgr::getTotalEntity()
{
	return m_entitys->getUsedEntity();
}