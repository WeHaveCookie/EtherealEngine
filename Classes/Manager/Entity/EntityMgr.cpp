#include "stdafx.h"
#include "EntityMgr.h"

#include "../../External/rapidjson/document.h"
#include "Entity/EntityPool.h"

EntityMgr::EntityMgr()
:Manager(ManagerType::Enum::Entity)
{
	m_entitys = new EntityPool(100);
}

EntityMgr::~EntityMgr()
{

}

void EntityMgr::init()
{
	buildEntity("Data/Character/player.json");
	buildEntity("Data/Character/chicken.json");
	buildEntity("Data/Character/cow.json");
	buildEntity("Data/Character/pig.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/client.json");
	buildEntity("Data/Character/entrepreneur.json");
	buildEntity("Data/Character/entrepreneur.json");
	buildEntity("Data/Character/entrepreneur.json");
	buildEntity("Data/Character/entrepreneur.json");
}

void EntityMgr::process(const float dt)
{
	m_entitys->process(dt);
}

void EntityMgr::end()
{
	free(m_entitys);
}

void EntityMgr::paint()
{
	m_entitys->paint();
}

void EntityMgr::buildEntity(const char* path)
{
	m_entitys->create(path);
}

Entity* EntityMgr::getEntity(uint32_t id)
{
	return m_entitys->getEntity(id);
}