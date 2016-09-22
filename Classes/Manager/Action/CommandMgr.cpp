#include "stdafx.h"
#include "CommandMgr.h"
#include "Actions/Command.h"
#include "Actions/CommandMove.h"

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
	{ "CommandMoveYAxis", CommandType::MoveYAxis }
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
}

void CommandMgr::process(const float dt)
{
	// If u want to keep a cmd pool, we need to create a specific stack ! Necessary or not ? This is the question dude !
	Command* cmd;
	bool dequeue = m_CommandQueue->try_dequeue(cmd);
	while (dequeue)
	{
		cmd->execute();
		//free(cmd);
		dequeue = m_CommandQueue->try_dequeue(cmd);
	}
}

void CommandMgr::end()
{

}

void CommandMgr::addCommand(Command* command)
{
	m_CommandQueue->enqueue(command);
}

Command* CommandMgr::getCommand(const char* cmd)
{
	auto command = m_commands[StringToCommandType[cmd]];
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
