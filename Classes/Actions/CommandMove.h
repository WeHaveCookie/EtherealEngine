#pragma once
#include "Command.h"

#define NAME_CMD(CommandClass) : CommandMove(#CommandClass) 

class Entity;

class CommandMove : public Command
{
public:
	CommandMove(const char* name) :Command(name, CommandExeType::Pressed) {};
	~CommandMove() {};
	
	virtual void init(Entity* ent, void* data);
	virtual void execute();
	virtual void undo();
	virtual void* makeCopy() = 0;

private:
	Vector2 m_lastPosition;
	Vector2 m_motion;
};

class CommandMoveLeft : public CommandMove
{
public:
	CommandMoveLeft() NAME_CMD(CommandMoveLeft) {};
	~CommandMoveLeft() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void* makeCopy();
};

class CommandMoveRight : public CommandMove
{
public:
	CommandMoveRight() NAME_CMD(CommandMoveRight) {};
	~CommandMoveRight() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void* makeCopy();
};

class CommandMoveUp : public CommandMove
{
public:
	CommandMoveUp() NAME_CMD(CommandMoveUp) {};
	~CommandMoveUp() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void* makeCopy();
};

class CommandMoveDown : public CommandMove
{
public:
	CommandMoveDown() NAME_CMD(CommandMoveDown) {};
	~CommandMoveDown() {};

	virtual void init(Entity* ent, void* data = NULL);
	virtual void* makeCopy();
};

class CommandMoveXAxis : public CommandMove
{
public:
	CommandMoveXAxis() NAME_CMD(CommandMoveXAxis) {};
	~CommandMoveXAxis() {};

	virtual void init(Entity* ent, void* data);
	virtual void* makeCopy();
};

class CommandMoveYAxis : public CommandMove
{
public:
	CommandMoveYAxis() NAME_CMD(CommandMoveYAxis) {};
	~CommandMoveYAxis() {};

	virtual void init(Entity* ent, void* data);
	virtual void* makeCopy();
};