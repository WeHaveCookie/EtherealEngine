#pragma once
#include "Command.h"


class CommandRotate: public Command
{
public:
	CommandRotate() : Command("CommandRotate", CommandExeType::JustPressed) {};
	~CommandRotate();

	virtual void init(Entity* ent, void* data = NULL);
	virtual bool execute();
	virtual void undo();
	virtual void* makeCopy();

};

