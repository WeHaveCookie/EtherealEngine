#pragma once

#include "Command.h"

class CommandJump : public Command
{
public:	
	CommandJump() : Command("CommandJump", CommandExeType::JustPressed) {};
	~CommandJump() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void execute();
	virtual void undo();
	virtual void* makeCopy();

private:
	Vector2 m_motion;
};
