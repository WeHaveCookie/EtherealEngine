#include "stdafx.h"

#include "EtherealDreamManagers.h"
#include "Manager/Game/GameMgr.h"

int main()
{
	EtherealDreamManagers* gameMgrs = new EtherealDreamManagers();
	gameMgrs->CreateManagers();
	gameMgrs->InitManagers();
	sf::Clock framerate;
	ImGui::SFML::Init(*GameMgr::getSingleton()->getMainRenderWindow());

	while (gameMgrs->isRunning())
	{
		gameMgrs->UpdateManagers(g_DeltaTime);
		g_DeltaTime = framerate.restart().asSeconds();
		g_Framerate = 1.0f / g_DeltaTime;
	}
	
	ImGui::SFML::Shutdown();
	gameMgrs->DestroyManagers();

    return 0;
}
