#pragma once
#include "Actions/Command.h"

class CommandExit : public Command
{
public:
	CommandExit() : Command("CommandExit", CommandExeType::AtOnce) {};
	~CommandExit();

	virtual void init(Entity* ent, void* data) {};
	virtual bool execute();
	virtual void undo() {};
	virtual void* makeCopy();
};

