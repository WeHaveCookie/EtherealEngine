#include "stdafx.h"
#include "RenderMgr.h"

#include "EtherealDreamManagers.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Game/GameMgr.h"

RenderMgr::RenderMgr()
:Manager(ManagerType::Enum::Render)
{

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
	levelMgr->paint();
	entityMgr->paint();
	endFrame();
}

void RenderMgr::end()
{

}

void RenderMgr::startFrame()
{
	m_mainRenderWindow->clear();
	//ImGui::NewFrame();
}

void RenderMgr::endFrame()
{
	//ImGui::Render();
	m_mainRenderWindow->display();
}