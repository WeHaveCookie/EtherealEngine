#pragma once
#include "Actions/Command.h"
#include "Manager/Entity/EntityMgr.h"

class CommandShoot : public Command
{
	public:
		CommandShoot() : Command("CommandShoot", CommandExeType::JustReleased) {};
		~CommandShoot();

		virtual void init(Entity* ent , void* data);
		virtual bool execute();
		virtual void undo() {};
		virtual void* makeCopy();

	private:
		ShootType::Enum		m_shootType;
};

class CommandShootBlue : public CommandShoot
{
	public:
		CommandShootBlue() {};
		~CommandShootBlue();

		virtual void init(Entity* ent, void* data) { Command::initDone(); };
		virtual bool execute();
		virtual void undo() {};
		virtual void* makeCopy();
		
};

class CommandShootGreen : public CommandShoot
{
public:
	CommandShootGreen() {};
	~CommandShootGreen();

	virtual void init(Entity* ent, void* data) { Command::initDone(); };
	virtual bool execute();
	virtual void undo() {};
	virtual void* makeCopy();

};

class CommandShootRed : public CommandShoot
{
public:
	CommandShootRed() {};
	~CommandShootRed();

	virtual void init(Entity* ent, void* data) { Command::initDone(); };
	virtual bool execute();
	virtual void undo() {};
	virtual void* makeCopy();

};