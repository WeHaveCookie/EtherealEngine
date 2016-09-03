#include "stdafx.h"

#include "EtherealDreamManagers.h"


int main()
{
	EtherealDreamManagers* gameMgrs = new EtherealDreamManagers();
	gameMgrs->CreateManagers();
	gameMgrs->InitManagers();
   

	while (gameMgrs->isRunning())
	{
		gameMgrs->UpdateManagers(g_DeltaTime);
	}
	
	gameMgrs->DestroyManagers();

    return 0;
}
