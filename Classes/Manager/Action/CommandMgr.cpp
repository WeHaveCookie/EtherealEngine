#include "stdafx.h"
#include "CommandMgr.h"
#include "Actions/Command.h"
#include "Actions/CommandMove.h"
#include "Actions/CommandJump.h"
#include "Actions/CommandAttack.h"
#include "Actions/CommandSound.h"
#include "Actions/CommandInput.h"
#include "Actions/CommandSpawn.h"
#include "Actions/CommandChannel.h"
#include "Actions/CommandShoot.h"
#include "Actions/CommandRotate.h"
#include "Actions/CommandLoadLevel.h"
#include "Actions/CommandExit.h"
#include "Actions/CommandShake.h"

#include "blockingconcurrentqueue.h"

#define CREATE_CMD(CommandClass) m_commands[StringToCommandType[#CommandClass]] = new CommandClass();

CommandMgr* CommandMgr::s_singleton = NULL;

class CommandMgr::CommandQueue : public moodycamel::BlockingConcurrentQueue<Command*> {};

std::map<std::string, CommandType::Enum> StringToCommandType =
{
	{ "CommandMoveRight", CommandType::MoveRight},
	{ "CommandMoveUp", CommandType::MoveUp},
	{ "CommandMoveLeft", CommandType::MoveLeft},
	{ "CommandMoveDown", CommandType::MoveDown},
	{ "CommandMoveXAxis", CommandType::MoveXAxis },
	{ "CommandMoveYAxis", CommandType::MoveYAxis },
	{ "CommandJump", CommandType::Jump},
	{ "CommandAttack", CommandType::Attack},
	{ "CommandSound", CommandType::Sound},
	{ "CommandLockInput", CommandType::LockInput},
	{ "CommandUnlockInput", CommandType::UnlockInput},
	{ "CommandSpawn", CommandType::Spawn},
	{ "CommandChannelUpgrade", CommandType::ChannelUpgrade },
	{ "CommandChannelDowngrade", CommandType::ChannelDowngrade },
	{ "CommandShoot", CommandType::Shoot },
	{ "CommandRotate", CommandType::Rotate},
	{ "CommandLoadLevel", CommandType::LoadLevel },
	{ "CommandExit", CommandType::Exit},
	{ "CommandShake", CommandType::Shake},
	{ "CommandMoveSprite", CommandType::MoveSprite},
	{ "CommandShootBlue", CommandType::ShootBlue},
	{ "CommandShootGreen", CommandType::ShootGreen},
	{ "CommandShootRed", CommandType::ShootRed}
};

std::vector<const char*> CommandTypeToString =
{
	"None",
	"MoveRight",
	"MoveUp",
	"MoveLeft",
	"MoveDown",
	"MoveXAxis",
	"MoveYAxis",
	"Jump",
	"Attack",
	"Sound",
	"LockInput",
	"UnlockInput",
	"Spawn",
	"ChannelUpgrade",
	"ChannelDowngrade",
	"Shoot",
	"Rotate",
	"LoadLevel",
	"Exit",
	"Shake",
	"MoveSprite",
	"ShootBlue",
	"ShootGreen",
	"ShootRed"
};

CommandMgr::CommandMgr()
	:Manager(ManagerType::Command)
{
	s_singleton = this;
	m_CommandQueue = new CommandQueue();
}

CommandMgr::~CommandMgr()
{
	delete m_CommandQueue;
}

void CommandMgr::init()
{
	CREATE_CMD(CommandMoveLeft)
		CREATE_CMD(CommandMoveRight)
		CREATE_CMD(CommandMoveUp)
		CREATE_CMD(CommandMoveDown)
		CREATE_CMD(CommandMoveYAxis)
		CREATE_CMD(CommandMoveXAxis)
		CREATE_CMD(CommandJump)
		CREATE_CMD(CommandAttack)
		CREATE_CMD(CommandSound)
		CREATE_CMD(CommandLockInput)
		CREATE_CMD(CommandUnlockInput)
		CREATE_CMD(CommandSpawn)
		CREATE_CMD(CommandChannelUpgrade)
		CREATE_CMD(CommandChannelDowngrade)
		CREATE_CMD(CommandShoot)
		CREATE_CMD(CommandRotate)
		CREATE_CMD(CommandLoadLevel)
		CREATE_CMD(CommandExit)
		CREATE_CMD(CommandShake)
		CREATE_CMD(CommandMoveSprite)
		CREATE_CMD(CommandShootBlue)
		CREATE_CMD(CommandShootGreen)
		CREATE_CMD(CommandShootRed)
}

void CommandMgr::process(const float dt)
{
	// If u want to keep a cmd pool, we need to create a specific stack ! Necessary or not ? This is the question dude !
	std::vector<Command*> m_CommandUnexecuted;
	Command* cmd;
	bool dequeue = m_CommandQueue->try_dequeue(cmd);
	while (dequeue)
	{
		if (!cmd->execute())
		{
			m_CommandUnexecuted.push_back(cmd);
		}
		//free(cmd);
		dequeue = m_CommandQueue->try_dequeue(cmd);
	}

	for (auto& cmdUn : m_CommandUnexecuted)
	{
		m_CommandQueue->enqueue(cmdUn);
	}
}

void CommandMgr::end()
{

}

void CommandMgr::addCommand(Command* command)
{
	m_CommandQueue->enqueue(command);
}

Command* CommandMgr::getCommand(const char* cmd, int* id)
{
	auto command = m_commands[StringToCommandType[cmd]];
	*id = StringToCommandType[cmd];
	if (command != NULL)
	{
		return (Command*)command->makeCopy();
	}
	return NULL;
}

Command* CommandMgr::getCommand(int id)
{
	auto command = m_commands[static_cast<CommandType::Enum>(id)];
	if (command != NULL)
	{
		return (Command*)command->makeCopy();
	}
	return NULL;
}

void CommandMgr::showImGuiWindow(bool* window)
{
	if (ImGui::Begin("CommandMgr", window))
	{

	}
	ImGui::End();
}

char** CommandMgr::getCommandsLabel(int* size) const
{
	*size = CommandTypeToString.size();
	char** label = (char**)malloc(sizeof(char*)*CommandTypeToString.size());

	int i = 0;
	for (auto& key : CommandTypeToString)
	{
		label[i] = (char*)malloc(sizeof(char)*strlen(key)+ 1); // For null terminated
		strcpy(label[i], key);
		i++;
	}

	return label;
}

void CommandMgr::releaseCommands()
{
	Command* cmd;
	bool dequeue = m_CommandQueue->try_dequeue(cmd);
	while (dequeue)
	{
		dequeue = m_CommandQueue->try_dequeue(cmd);
	}
}