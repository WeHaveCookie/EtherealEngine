#include "stdafx.h"
#include "CommandAttack.h"
#include "Entity/Entity.h"

void CommandAttack::init(Entity* ent, void* data)
{
	Command::init(ent);
}

void CommandAttack::execute()
{
	Entity* entity = getEntity();
	if (!entity->isAttack())
	{
		entity->attack();
	}
}

void CommandAttack::undo()
{

}

void* CommandAttack::makeCopy()
{
	return (void*)new CommandAttack();
}