#pragma once

#include "Command.h"

class CommandAttack : public Command
{
public:
	CommandAttack() : Command("CommandAttack", CommandExeType::JustPressed) {};
	~CommandAttack() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void execute();
	virtual void undo();
	virtual void* makeCopy();

private:
};
