#include "stdafx.h"
#include "CommandShake.h"
#include "Manager/Level/LevelMgr.h"
#include "EtherealEngineManagers.h"
#include "Manager/Sound/SoundMgr.h"

CommandShake::~CommandShake()
{
}

void CommandShake::init(Entity* ent, void* data) 
{
	float* timer = static_cast<float*>(data);
	if (timer != nullptr)
	{
		m_timer = *timer;
		Command::setExeType(CommandExeType::AtTime);
	}
	else
	{
		m_timer = 0.0f;
	}
}


bool CommandShake::execute()
{
	m_timer -= g_DeltaTime;
	if (m_timer <= 0.0)
	{
		LevelMgr::getSingleton()->shake(true);
		SoundMgr::getSingleton()->addSound("Data/Sound/FX/Shake.ogg");
		return true;
	}
	else
	{
		return false;
	}
}

void* CommandShake::makeCopy()
{
	return (void*)new CommandShake();
}
