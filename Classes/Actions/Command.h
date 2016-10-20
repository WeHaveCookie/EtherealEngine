#pragma once

class Entity;

namespace CommandExeType
{
	enum Enum
	{
		None = 0,
		JustPressed,
		Pressed,
		JustReleased,
		Released,
		AtOnce
	};
}

class Command
{
public:
	Command(const char* name, CommandExeType::Enum exeType);
	~Command();

	virtual void init(Entity* ent = NULL, void* data = NULL);
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual void* makeCopy() = 0;
	
	Entity* getEntity() { return m_entity; }
	const char* getName() const { return m_name; }
	const CommandExeType::Enum getExeType() const { return m_exeType; }

private:
	Entity*					m_entity;
	const char*				m_name;
	CommandExeType::Enum	m_exeType;
};