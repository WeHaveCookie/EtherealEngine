#pragma once
#include "Actions/Command.h"

class CommandShake : public Command
{
public:
	CommandShake() : Command("CommandShake", CommandExeType::AtOnce) { };
	~CommandShake();

	virtual void init(Entity* ent, void* data);
	virtual bool execute();
	virtual void undo() {};
	virtual void* makeCopy();

private:
	float	m_timer;
};

