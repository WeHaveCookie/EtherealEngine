#pragma once
#include "Actions/Command.h"

class CommandSpawn : public Command
{
	struct SpawnHandler {
		std::string		path;
		sf::Vector2f	position;
		uint32_t		timer;
		bool			valid;
	};

	public:
		CommandSpawn() : Command("CommmandSpawn", CommandExeType::AtTime) {};
		~CommandSpawn();

		virtual void init(Entity* ent, void* data = NULL);
		virtual bool execute();
		virtual void undo();
		virtual void* makeCopy();

	private:
		SpawnHandler	m_spawnHandler;
};