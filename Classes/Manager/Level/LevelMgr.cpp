#include "stdafx.h"
#include "LevelMgr.h"
#include "Level/Quadtree.h"
#include "Entity/Entity.h"

LevelMgr* LevelMgr::s_singleton = NULL;

LevelMgr::LevelMgr()
:Manager(ManagerType::Enum::Level)
{
	s_singleton = this;
}

LevelMgr::~LevelMgr()
{
}

void LevelMgr::init()
{
	m_quadtree = new Quadtree();
	m_quadtree->init(0.0f, 0.0f, 1920.0f, 1080.0f);
	m_quadtree->setNodeCapacity(30);
}

void LevelMgr::process(const float dt)
{
	sf::Clock clock;
	m_quadtree->update();
	m_processTime = clock.getElapsedTime();
}

void LevelMgr::end()
{

}

void LevelMgr::paint()
{
	m_quadtree->paint();
}

void LevelMgr::registerEntity(Entity* ent)
{
	m_quadtree->registerEntity(ent);
}

void LevelMgr::unregisterEntity(uint32_t id)
{
	m_quadtree->unregisterEntity(id);
}

std::vector<Entity*> LevelMgr::getEntityAround(sf::FloatRect bound)
{
	m_queryCount++;
	return m_quadtree->queryRange(bound);
}

int LevelMgr::getRegisterCount() 
{ 
	return m_quadtree->getRegisterCount(); 
}

int LevelMgr::getUnregisterCount() 
{ 
	return m_quadtree->getUnregisterCount(); 
}

int LevelMgr::getMasterRegisterCount()
{
	return m_quadtree->getMasterRegisterCount();
}

int LevelMgr::getQueryCount()
{
	return m_quadtree->getQueryCount();
}