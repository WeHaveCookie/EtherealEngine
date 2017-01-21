#include "stdafx.h"
#include "CommandAttack.h"
#include "Entity/Entity.h"

void CommandAttack::init(Entity* ent, void* data)
{
	Command::init(ent);
}

bool CommandAttack::execute()
{
	Entity* entity = getEntity();
	if (!entity->isAttack())
	{
		entity->attack();
	}
	return true;
}

void CommandAttack::undo()
{

}

void* CommandAttack::makeCopy()
{
	return (void*)new CommandAttack();
}