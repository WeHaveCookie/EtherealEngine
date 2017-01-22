#include "stdafx.h"
#include "CommandLoadLevel.h"
#include "Manager/Level/LevelMgr.h"

CommandLoadLevel::~CommandLoadLevel()
{
}

void CommandLoadLevel::init(Entity* ent, void* data)
{
	std::string* levelPath = static_cast<std::string*>(data);
	if (levelPath != nullptr)
	{
		m_levePath = *levelPath;
		Command::initDone();
	}
}

bool CommandLoadLevel::execute()
{
	LevelMgr::getSingleton()->loadLevel((char*)m_levePath.c_str());
	return true;
}

void* CommandLoadLevel::makeCopy()
{
	return (void*)new CommandLoadLevel();
}