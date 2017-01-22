#include "stdafx.h"
#include "RenderMgr.h"

#include "EtherealEngineManagers.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/GuiMgr.h"
#include "Level/Quadtree.h"

RenderMgr* RenderMgr::s_singleton = NULL;

RenderMgr::RenderMgr()
:Manager(ManagerType::Enum::Render)
{
	s_singleton = this;
}

RenderMgr::~RenderMgr()
{

}

void RenderMgr::init()
{
	auto gameMgr = GAME_MGR;
	m_mainRenderWindow = gameMgr->getMainRenderWindow();
}

void RenderMgr::process(const float dt)
{
	auto entityMgr = ENTITY_MGR;
	auto levelMgr = LEVEL_MGR;
	m_mainRenderWindow->setView(levelMgr->getCamera());
	levelMgr->paint();
	entityMgr->paint();
	GuiMgr::getSingleton()->paint();

	endFrame();
}

void RenderMgr::end()
{

}

void RenderMgr::startFrame()
{
	m_mainRenderWindow->clear();
	m_timeFrame.restart();
}

void RenderMgr::endFrame()
{
	ImGui::Render();
	auto timer = m_timeFrame.getElapsedTime().asMilliseconds();
	
	m_mainRenderWindow->display();
}