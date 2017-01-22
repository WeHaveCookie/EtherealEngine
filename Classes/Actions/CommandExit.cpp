#include "stdafx.h"
#include "CommandExit.h"
#include "Manager/Game/GameMgr.h"

CommandExit::~CommandExit()
{
}

void CommandExit::init(Entity* ent, void* data) 
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
