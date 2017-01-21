#include "stdafx.h"
#include "CommandJump.h"
#include "Entity/Entity.h"

void CommandJump::init(Entity* ent, void* data)
{
	Command::init(ent);
}

bool CommandJump::execute()
{
	Entity* entity = getEntity();
	if (!entity->isFall())
	{
		entity->jump();
	}
	return true;
}

void CommandJump::undo()
{

}

void* CommandJump::makeCopy()
{
	return (void*)new CommandJump();
}