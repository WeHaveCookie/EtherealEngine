#pragma once
#include "Actions/Command.h"

class CommandLoadLevel : public Command
{
	public:
		CommandLoadLevel() : Command("CommandLoadLevel", CommandExeType::AtOnce) {};
		~CommandLoadLevel();

		virtual void init(Entity* ent, void* data);
		virtual bool execute();
		virtual void undo() { };
		virtual void* makeCopy();

	private:
		std::string m_levePath;
};

