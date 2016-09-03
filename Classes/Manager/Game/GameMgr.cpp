#include "stdafx.h"
#include "GameMgr.h"
#include "EtherealDreamManagers.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Entity/EntityMgr.h"

GameMgr::GameMgr()
:Manager(ManagerType::Enum::Game)
{

}

GameMgr::~GameMgr()
{
}

void GameMgr::init()
{
	sf::VideoMode vm = sf::VideoMode::getDesktopMode();
	std::cout << vm.height << " " << vm.width << std::endl;
	if (vm.height > 1080 || vm.width > 1920)
	{
		vm = sf::VideoMode(1920, 1080);
	}
	m_mainRenderWindow = new sf::RenderWindow(vm, "Ethereal Dream", sf::Style::Default /*| sf::Style::Fullscreen*/);
	setFrameRate(60);
	m_mainRenderWindow->setVerticalSyncEnabled(true);
	srand(time(NULL));

	sf::Joystick::update();

}

void GameMgr::process(const float dt)
{
	auto inputMgr = INPUT_MGR;
	auto entityMgr = ENTITY_MGR;
	sf::Vector2f motion = { 0.0f, 0.0f };
// 	auto ent = entityMgr->getEntity(1);
// 	
// 	if (inputMgr->keyIsJustPressed(KeyType::kbRight))
// 	{
// 		motion.x++;
// 		ent->setState(EntityAnimationState::Right);
// 	}
// 	if (inputMgr->keyIsPressed(KeyType::kbRight))
// 	{
// 		motion.x++;
// 	}
// 
// 	if (inputMgr->keyIsJustReleased(KeyType::kbRight))
// 	{
// 		ent->setState(EntityAnimationState::IdleRight);
// 	}
// 
// 	if (inputMgr->keyIsJustPressed(KeyType::kbLeft))
// 	{
// 		motion.x--;
// 		ent->setState(EntityAnimationState::Left);
// 	}
// 	if (inputMgr->keyIsPressed(KeyType::kbLeft))
// 	{
// 		motion.x--;
// 	}
// 
// 	if (inputMgr->keyIsJustReleased(KeyType::kbLeft))
// 	{
// 		ent->setState(EntityAnimationState::IdleLeft);
// 	}
// 
// 	if (inputMgr->keyIsPressed(KeyType::kbUp))
// 	{
// 		motion.y--;
// 	}
// 
// 	if (inputMgr->keyIsPressed(KeyType::kbDown))
// 	{
// 		motion.y++;
// 	}
// 	ent->move(motion);
	

}

void GameMgr::end()
{
}

void GameMgr::paint()
{
}

bool GameMgr::isRunning()
{
	return m_mainRenderWindow->isOpen();
}

void GameMgr::setFrameRate(unsigned int frameRate)
{
	m_mainRenderWindow->setFramerateLimit(frameRate);
	g_DeltaTime = 1.0f / (float)frameRate;
}