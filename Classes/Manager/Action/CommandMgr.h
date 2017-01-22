#pragma once
#include "Manager/Manager.h"
#include "Manager/Input/InputMgr.h"


class Command;

namespace CommandType
{
	enum Enum
	{
		None = 0,
		MoveRight,
		MoveUp,
		MoveLeft,
		MoveDown,
		MoveXAxis,
		MoveYAxis,
		Jump,
		Attack,
		Sound,
		LockInput,
		UnlockInput,
		Spawn,
		ChannelUpgrade,
		ChannelDowngrade,
		Shoot,
		Rotate,
		LoadLevel,
		Exit,
		Shake,
		MoveSprite,
		ShootBlue,
		ShootGreen,
		ShootRed
	};
}

class CommandMgr : public Manager
{
public:
	static CommandMgr* getSingleton() { return s_singleton; }

	CommandMgr();
	~CommandMgr();

	void init();
	void process(const float dt);
	void end();
	void addCommand(Command* command);
	Command* getCommand(const char* cmd, int* id);
	Command* getCommand(int id);

	void showImGuiWindow(bool* window);
	char** getCommandsLabel(int* size) const;
	void releaseCommands();

private:
	static CommandMgr*						s_singleton;

	class CommandQueue;
	CommandQueue*									m_CommandQueue;
	std::map<CommandType::Enum, Command*>			m_commands;

};