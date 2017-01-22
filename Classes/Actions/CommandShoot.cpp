#include "stdafx.h"
#include "CommandShoot.h"
#include "Utils/Random.h"
#include "Manager\Physic\PhysicMgr.h"
#include "Manager\Game\GameMgr.h"

CommandShoot::~CommandShoot()
{
}

void CommandShoot::init(Entity* ent, void* data)
{
	Command::init(ent);
	ShootType::Enum* shootType = static_cast<ShootType::Enum*>(data);
	if (shootType != nullptr)
	{
		m_shootType = *shootType;
	}
}

bool CommandShoot::execute()
{
	EntityMgr::getSingleton()->createShoot(m_shootType);
	return true;
}

void* CommandShoot::makeCopy()
{
	return (void*)new CommandShoot();
}

bool CommandShootBlue::execute()
{
	EntityMgr::getSingleton()->createShoot(ShootType::Triangle);
	return true;
}

void* CommandShootBlue::makeCopy()
{
	return (void*)new CommandShootBlue();
}

bool CommandShootGreen::execute()
{
	EntityMgr::getSingleton()->createShoot(ShootType::Spike);
	return true;
}

void* CommandShootGreen::makeCopy()
{
	return (void*)new CommandShootGreen();
}

bool CommandShootRed::execute()
{
	EntityMgr::getSingleton()->createShoot(ShootType::Sinus);
	return true;
}

void* CommandShootRed::makeCopy()
{
	return (void*)new CommandShootRed();
}