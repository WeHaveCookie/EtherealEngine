#pragma once

#include "Command.h"

class CommandLockInput : public Command
{
public:
	CommandLockInput() : Command("CommandLockInput", CommandExeType::AtOnce) {};
	~CommandLockInput() {};

	virtual void init(Entity* ent = NULL, void* data = NULL) {};
	virtual void execute();
	virtual void undo() {};
	virtual void* makeCopy() { return (void*)new CommandLockInput(); }

private:
};

class CommandUnlockInput : public Command
{
public:
	CommandUnlockInput() : Command("CommandUnlockInput", CommandExeType::AtOnce) {};
	~CommandUnlockInput() {};

	virtual void init(Entity* ent = NULL, void* data = NULL) {};
	virtual void execute();
	virtual void undo() {};
	virtual void* makeCopy() { return (void*)new CommandUnlockInput(); }

private:
};
