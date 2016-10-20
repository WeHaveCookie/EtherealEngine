#include "stdafx.h"
#include "CommandMove.h"
#include "Entity/Entity.h"
#include "Manager/Game/GameMgr.h"

void CommandMove::init(Entity* ent, void* data)
{
	Command::init(ent);
	m_motion = *static_cast<Vector2*>(data);
	free(data);
}

void CommandMove::execute()
{
	Entity* entity = getEntity();
	m_lastPosition = entity->getPosition();
	entity->addMotion(m_motion);
}

void CommandMove::undo()
{
	getEntity()->setPosition(m_lastPosition);
}

void CommandMoveLeft::init(Entity* ent, void* data)
{
	float vx;
	if (data != NULL)
	{
		vx = -*(float*)data;
	}
	else
	{
		vx = -(float)GameMgr::getSingleton()->getMovementSpeed();
	}
	CommandMove::init(ent, (void*)new Vector2(vx, 0.0f));
}

void* CommandMoveLeft::makeCopy()
{
	return (void*)new CommandMoveLeft();
}

void CommandMoveRight::init(Entity* ent, void* data)
{
	float vx;
	if (data != NULL)
	{
		vx = *(float*)data;
	}
	else
	{
		vx = (float)GameMgr::getSingleton()->getMovementSpeed();
	}
	CommandMove::init(ent, (void*)new Vector2(vx, 0.0f));
}

void* CommandMoveRight::makeCopy()
{
	return (void*)new CommandMoveRight();
}

void CommandMoveUp::init(Entity* ent, void* data)
{
	float vy;
	if (data != NULL)
	{
		vy = -*(float*)data;
	}
	else
	{
		vy = -(float)GameMgr::getSingleton()->getMovementSpeed();
	}
	CommandMove::init(ent, (void*)new Vector2(0.0f, vy));
}

void* CommandMoveUp::makeCopy()
{
	return (void*)new CommandMoveUp();
}

void CommandMoveDown::init(Entity* ent, void* data)
{
	float vy;
	if (data != NULL)
	{
		vy = *(float*)data;
	}
	else
	{
		vy = (float)GameMgr::getSingleton()->getMovementSpeed();
	}
	CommandMove::init(ent, (void*)new Vector2(0.0f, vy));
}

void* CommandMoveDown::makeCopy()
{
	return (void*)new CommandMoveDown();
}

void CommandMoveXAxis::init(Entity* ent, void* data)
{
	CommandMove::init(ent, (void*)new Vector2(*static_cast<float*>(data), 0.0f));
}

void* CommandMoveXAxis::makeCopy()
{
	return (void*)new CommandMoveXAxis();
}

void CommandMoveYAxis::init(Entity* ent, void* data)
{
	CommandMove::init(ent, (void*)new Vector2(0.0f, *static_cast<float*>(data)));
}

void* CommandMoveYAxis::makeCopy()
{
	return (void*)new CommandMoveYAxis();
}
