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
		virtual void undo();
		virtual void* makeCopy();

	private:
		ShootType::Enum		m_shootType;
};
