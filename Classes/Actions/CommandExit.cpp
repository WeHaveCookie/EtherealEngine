#include "stdafx.h"
#include "CommandExit.h"
#include "Manager/Game/GameMgr.h"

CommandExit::~CommandExit()
{
}

bool CommandExit::execute()
{
	GameMgr::getSingleton()->getMainRenderWindow()->close();
	return true;
}

void* CommandExit::makeCopy()
{
	return (void*)new CommandExit();
}
