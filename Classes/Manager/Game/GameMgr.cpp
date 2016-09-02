#include "stdafx.h"
#include "GameMgr.h"


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
	m_mainRenderWindow->setFramerateLimit(60);
	m_mainRenderWindow->setVerticalSyncEnabled(true);
	srand(time(NULL));

	sf::Joystick::update();

}

void GameMgr::process(const float dt)
{
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