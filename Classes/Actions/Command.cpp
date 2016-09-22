#include "stdafx.h"
#include "Command.h"
#include "Entity/Entity.h"

Command::Command(const char* name, CommandExeType::Enum exeType)
	:m_name(name),
	m_exeType(exeType)
{

}

Command::~Command()
{
}

void Command::init(Entity* ent, void* data)
{
	m_entity = ent;
}