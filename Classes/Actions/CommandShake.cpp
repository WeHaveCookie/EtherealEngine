#include "stdafx.h"
#include "CommandShake.h"
#include "Manager/Level/LevelMgr.h"

CommandShake::~CommandShake()
{
}

bool CommandShake::execute()
{
	LevelMgr::getSingleton()->shake(true);
	return true;
}

void* CommandShake::makeCopy()
{
	return (void*)new CommandShake();
}
