#include "stdafx.h"
#include "CommandMove.h"
#include "Entity/Entity.h"
#include "Manager/Game/GameMgr.h"
#include "EtherealEngineManagers.h"

void CommandMove::init(Entity* ent, void* data)
{
	Command::init(ent);
	Vector2* motion = static_cast<Vector2*>(data);
	if (motion != nullptr)
	{
		m_motion = *motion;
	}
	free(data);
}

bool CommandMove::execute()
{
	Entity* entity = getEntity();
	m_lastPosition = entity->getPosition();
	entity->addMotion(m_motion);
	return true;
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

void CommandMoveSprite::init(Entity* ent, void* data)
{
	MoveSpriteHandler* handle = static_cast<MoveSpriteHandler*>(data);
	if (handle != NULL)
	{
		m_moveSpriteHandler = *handle;
	}
}

bool CommandMoveSprite::execute()
{
	m_moveSpriteHandler.m_timer -= g_DeltaTime;
	if (m_moveSpriteHandler.m_timer <= 0.0)
	{
		m_moveSpriteHandler.m_sprite->move(m_moveSpriteHandler.m_motion);
		return true;
	}
	else
	{
		return false;
	}
}

void* CommandMoveSprite::makeCopy()
{
	return (void*)new CommandMoveSprite();
}