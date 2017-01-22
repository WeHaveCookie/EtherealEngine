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
	// TEST
	m_shootType = (ShootType::Enum)randIntBorned(1, 4);
	EntityMgr::getSingleton()->createShoot(m_shootType);
	return true;
}

void* CommandShoot::makeCopy()
{
	return (void*)new CommandShoot();
}