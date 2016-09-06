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
	m_quadtree->setNodeCapacity(4);
}

void LevelMgr::process(const float dt)
{
	m_quadtree->update();
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

std::vector<Entity*> LevelMgr::getEntityAround(sf::FloatRect bound)
{
	return m_quadtree->queryRange(bound);
}