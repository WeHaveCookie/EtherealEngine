#include "stdafx.h"
#include "CommandSpawn.h"
#include "Manager/Entity/EntityMgr.h"
#include "EtherealEngineManagers.h"
#include "Manager/Level/LevelMgr.h"

CommandSpawn::~CommandSpawn()
{
}

void CommandSpawn::init(Entity* ent, void* data)
{
	SpawnHandler* spawnHandler = static_cast<SpawnHandler*>(data);
	if (spawnHandler != nullptr)
	{
		m_spawnHandler = *spawnHandler;
		m_spawnHandler.valid = true;
	}
	else
	{
		m_spawnHandler.valid = false;
	}
}

bool CommandSpawn::execute()
{
	m_spawnHandler.timer -= g_DeltaTime;
	if (m_spawnHandler.timer <= 0.0)
	{
		if (m_spawnHandler.valid)
		{
			auto ent = EntityMgr::getSingleton()->createEntity(m_spawnHandler.path.c_str());
			EntityMgr::getSingleton()->spawnIntoRegion(ent->getUID(), m_spawnHandler.spawnRegion);
			LevelMgr::getSingleton()->registerEntityIntoLevel(ent);
		}
		return true;
	}
	else
	{
		return false;
	}
	
}

void CommandSpawn::undo()
{

}

void* CommandSpawn::makeCopy()
{
	return (void*)new CommandSpawn();
}