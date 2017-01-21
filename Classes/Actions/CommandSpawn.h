#pragma once
#include "Actions/Command.h"

struct SpawnHandler {
	std::string		path;
	sf::FloatRect	spawnRegion;
	float			timer;
	bool			valid;
};

class CommandSpawn : public Command
{

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