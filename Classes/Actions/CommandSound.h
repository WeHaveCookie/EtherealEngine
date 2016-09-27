#pragma once

#include "Command.h"

class CommandSound : public Command
{
public:
	CommandSound() : Command("CommandSound", CommandExeType::JustPressed) {};
	~CommandSound() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void execute();
	virtual void undo();
	virtual void* makeCopy();

private:
	uint32_t m_soundID;
};
