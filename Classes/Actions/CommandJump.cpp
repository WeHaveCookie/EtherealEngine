#include "stdafx.h"
#include "CommandJump.h"
#include "Entity/Entity.h"

void CommandJump::init(Entity* ent, void* data)
{
	Command::init(ent);
	if (ent->getOrientation() == EntityOrientation::Left)
	{
		m_motion = Vector2(-10.0f, 200.0f);
	}
	else
	{
		m_motion = Vector2(10.0f, 200.0f);
	}
}

void CommandJump::execute()
{
	Entity* entity = getEntity();
	if (!entity->isFall())
	{
		entity->addMotion(m_motion);
	}
}

void CommandJump::undo()
{

}

void* CommandJump::makeCopy()
{
	return (void*)new CommandJump();
}