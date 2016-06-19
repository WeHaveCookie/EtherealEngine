#include "stdafx.h"

#include "EtherealDreamManagers.h"


int main()
{
    sf::VideoMode vm = sf::VideoMode::getDesktopMode();
    std::cout << vm.height << " " << vm.width << std::endl;
    if(vm.height>1080 || vm.width > 1920)
    {
        vm = sf::VideoMode(1920,1080);
    }
    sf::RenderWindow* window = new sf::RenderWindow(vm, "Ethereal Dream", sf::Style::Default /*| sf::Style::Fullscreen*/);
    window->setFramerateLimit(60);
    window->setVerticalSyncEnabled(true);
    srand(time(NULL));

	EtherealDreamManagers* gameMgrs = new EtherealDreamManagers();
    sf::Joystick::update();

	gameMgrs->CreateManagers();
	gameMgrs->InitManagers();


	while (window->isOpen())
	{
		gameMgrs->UpdateManagers(g_DeltaTime);
	}
	
	gameMgrs->DestroyManagers();

    return 0;
}
