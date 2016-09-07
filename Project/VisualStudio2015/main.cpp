#include "stdafx.h"

#include "EtherealDreamManagers.h"

int main()
{
	EtherealDreamManagers* gameMgrs = new EtherealDreamManagers();
	gameMgrs->CreateManagers();
	gameMgrs->InitManagers();
	sf::Clock framerate;

	while (gameMgrs->isRunning())
	{
		gameMgrs->UpdateManagers(g_DeltaTime);

		g_Framerate = 1000.0f / framerate.restart().asMilliseconds();
	}
	
	gameMgrs->DestroyManagers();

    return 0;
}
