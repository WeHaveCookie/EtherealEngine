#include "stdafx.h"
#include "RenderMgr.h"

#include "EtherealDreamManagers.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Engine/PhysicMgr.h"
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
	m_font;
	if (!m_font.loadFromFile("data/Fonts/wonder.ttf"))
	{
	}

	m_text.setFont(m_font);

	m_text.setCharacterSize(24); // exprimée en pixels, pas en points !

	m_text.setColor(sf::Color::Magenta);

}

void RenderMgr::process(const float dt)
{
	auto entityMgr = ENTITY_MGR;
	auto levelMgr = LEVEL_MGR;
	levelMgr->paint();
	entityMgr->paint();
	
	m_text.setPosition(sf::Vector2f(0.0f, 20.0f));
	m_text.setString(std::to_string((int)g_Framerate));
	m_mainRenderWindow->draw(m_text);

	m_text.setPosition(sf::Vector2f(0.0f, 50.0f));
	m_text.setString(std::to_string(EntityMgr::getSingleton()->getTotalEntity()));
	m_mainRenderWindow->draw(m_text);

	static float maxPhysic = 0.0f; 
	float physic = PhysicMgr::getSingleton()->getProcessTime().asMilliseconds();
	if (maxPhysic < physic)
	{
		maxPhysic = physic;
	}
	m_text.setPosition(sf::Vector2f(0.0f, 80.0f));
	m_text.setString(std::to_string((int)physic));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(40.0f, 80.0f));
	m_text.setString(std::to_string((int)maxPhysic));
	m_mainRenderWindow->draw(m_text);


	static float maxLvl = 0.0f;
	float lvl = LevelMgr::getSingleton()->getProcessTime().asMilliseconds();
	if (maxLvl < lvl)
	{
		maxLvl = lvl;
	}
	m_text.setPosition(sf::Vector2f(0.0f, 110.0f));
	m_text.setString(std::to_string((int)lvl));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(40.0f, 110.0f));
	m_text.setString(std::to_string((int)maxLvl));
	m_mainRenderWindow->draw(m_text);

	static float maxEnt = 0.0f;
	float ent = EntityMgr::getSingleton()->getProcessTime().asMilliseconds();
	if (maxEnt < ent)
	{
		maxEnt = ent;
	}
	m_text.setPosition(sf::Vector2f(0.0f, 140.0f));
	m_text.setString(std::to_string((int)ent));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(40.0f, 140.0f));
	m_text.setString(std::to_string((int)maxEnt));
	m_mainRenderWindow->draw(m_text);


	static float maxMaster = 0.0f;
	float master = LevelMgr::getSingleton()->getMasterRegisterCount();
	if (maxMaster < master)
	{
		maxMaster = master;
	}
	m_text.setPosition(sf::Vector2f(0.0f, 200.0f));
	m_text.setString(std::to_string((int)master));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(100.0f, 200.0f));
	m_text.setString(std::to_string((int)maxMaster));
	m_mainRenderWindow->draw(m_text);

	static float maxRegi = 0.0f;
	float reg = LevelMgr::getSingleton()->getRegisterCount();
	if (maxRegi < reg)
	{
		maxRegi = reg;
	}
	m_text.setPosition(sf::Vector2f(300.0f, 200.0f));
	m_text.setString(std::to_string((int)reg));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(400.0f, 200.0f));
	m_text.setString(std::to_string((int)maxRegi));
	m_mainRenderWindow->draw(m_text);

	static float maxUnr = 0.0f;
	float unr = LevelMgr::getSingleton()->getUnregisterCount();
	if (maxUnr < unr)
	{
		maxUnr = unr;
	}
	m_text.setPosition(sf::Vector2f(0.0f, 230.0f));
	m_text.setString(std::to_string((int)unr));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(100.0f, 230.0f));
	m_text.setString(std::to_string((int)maxUnr));
	m_mainRenderWindow->draw(m_text);

	static float maxMQr = 0.0f;
	float mqr = LevelMgr::getSingleton()->getMasterQueryCount();
	if (maxMQr < mqr)
	{
		maxMQr = mqr;
	}
	m_text.setPosition(sf::Vector2f(0.0f, 260.0f));
	m_text.setString(std::to_string((int)mqr));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(100.0f, 260.0f));
	m_text.setString(std::to_string((int)maxMQr));
	m_mainRenderWindow->draw(m_text);

	static float maxQr = 0.0f;
	float qr = LevelMgr::getSingleton()->getQueryCount();
	if (maxQr  < qr)
	{
		maxQr = qr;
	}
	m_text.setPosition(sf::Vector2f(300.0f, 260.0f));
	m_text.setString(std::to_string((int)qr));
	m_mainRenderWindow->draw(m_text);
	m_text.setPosition(sf::Vector2f(400.0f, 260.0f));
	m_text.setString(std::to_string((int)maxQr));
	m_mainRenderWindow->draw(m_text);

	endFrame();
}

void RenderMgr::end()
{

}

void RenderMgr::startFrame()
{
	m_mainRenderWindow->clear();
	m_timeFrame.restart();
	//ImGui::NewFrame();
}

void RenderMgr::endFrame()
{
	//ImGui::Render();
	auto timer = m_timeFrame.getElapsedTime().asMilliseconds();
	//std::cout << timer << std::endl;
	
	m_mainRenderWindow->display();
}