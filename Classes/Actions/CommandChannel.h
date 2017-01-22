#pragma once
#include "Command.h"

#define NAME_CMD(CommandClass) : CommandChannel(#CommandClass) 

class CommandChannel : public Command
{

public:
	CommandChannel(const char* name) :Command(name, CommandExeType::AtOnce) {};
	~CommandChannel() {};

	virtual void init(Entity* ent = NULL, void* data = NULL) {};
	virtual bool execute() { return true; };
	virtual void undo() {};
	virtual void* makeCopy() = 0;


};

class CommandChannelUpgrade : public CommandChannel
{
public:
	CommandChannelUpgrade() NAME_CMD(CommandChannelUpgrade) {};
	~CommandChannelUpgrade() {};

	virtual bool execute();
	virtual void* makeCopy();


};

class CommandChannelDowngrade : public CommandChannel
{
public:
	CommandChannelDowngrade() NAME_CMD(CommandChannelDowngrade) {};
	~CommandChannelDowngrade() {};

	virtual bool execute();
	virtual void* makeCopy();


};