#include "stdafx.h"
#include "EtherealEngineManagers.h"

#include "Manager/Time/TimeMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Item/ItemMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Event/EventMgr.h"
#include "Manager/Physic/PhysicMgr.h"
#include "Manager/Render/RenderMgr.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/File/FileMgr.h"
#include "Manager/Persistent/PersistentMgr.h"
#include "Manager/Loading/LoadingMgr.h"
#include "Manager/Render/GuiMgr.h"
#include "Manager/Action/CommandMgr.h"

#include "Thread/LoadingThread.h"
#include "Thread/SaveThread.h"

EtherealEngineManagers* EtherealEngineManagers::m_instance = NULL;

#define REGISTER_MANAGER(ManagerClass) ManagerClass* g_##ManagerClass = NULL;

#define CREATE_MGR(ManagerClass) g_##ManagerClass = new ManagerClass(); m_managers.push_back(g_##ManagerClass);
#define INIT_MGR(ManagerClass) g_##ManagerClass->init();
#define PROCESS_MGR(ManagerClass) g_##ManagerClass->process(g_DeltaTime);
#define END_MGR(ManagerClass) g_##ManagerClass->end();	


float g_DeltaTimeRaw = 0.33f;
float g_DeltaTime = 0.33f;
float g_DeltaTimeFactor = 1.f;
float g_Framerate = 1.0f/60.0f;

REGISTER_MANAGER(TimeMgr)
REGISTER_MANAGER(SoundMgr)
REGISTER_MANAGER(LevelMgr)
REGISTER_MANAGER(ItemMgr)
REGISTER_MANAGER(InputMgr)
REGISTER_MANAGER(GameMgr)
REGISTER_MANAGER(EventMgr)
REGISTER_MANAGER(PhysicMgr)
REGISTER_MANAGER(RenderMgr)
REGISTER_MANAGER(EntityMgr)
REGISTER_MANAGER(FileMgr)
REGISTER_MANAGER(PersistentMgr)
REGISTER_MANAGER(LoadingMgr)
REGISTER_MANAGER(GuiMgr)
REGISTER_MANAGER(CommandMgr)

REGISTER_MANAGER(LoadingThread)
REGISTER_MANAGER(SaveThread)

EtherealEngineManagers::EtherealEngineManagers()
{
	m_instance = this;
}


void EtherealEngineManagers::CreateManagers()
{
	g_LoadingThread = new LoadingThread();
	g_SaveThread = new SaveThread();

	CREATE_MGR(InputMgr)
		CREATE_MGR(TimeMgr)
		CREATE_MGR(LoadingMgr)
		CREATE_MGR(SoundMgr)
		CREATE_MGR(LevelMgr)
		CREATE_MGR(ItemMgr)
		CREATE_MGR(GameMgr)
		CREATE_MGR(EventMgr)
		CREATE_MGR(CommandMgr)
		CREATE_MGR(PhysicMgr)
		CREATE_MGR(RenderMgr)
		CREATE_MGR(EntityMgr)
		CREATE_MGR(FileMgr)
		CREATE_MGR(PersistentMgr)
		CREATE_MGR(GuiMgr)
}

void EtherealEngineManagers::InitManagers()
{

	INIT_MGR(CommandMgr)
		INIT_MGR(InputMgr)
		INIT_MGR(TimeMgr)
		INIT_MGR(LoadingMgr)
		INIT_MGR(SoundMgr)
		INIT_MGR(LevelMgr)
		INIT_MGR(ItemMgr)
		INIT_MGR(PhysicMgr)
		INIT_MGR(GameMgr)
		INIT_MGR(EventMgr)
		INIT_MGR(EntityMgr)
		INIT_MGR(RenderMgr)
		INIT_MGR(FileMgr)
		INIT_MGR(PersistentMgr)
		INIT_MGR(GuiMgr)
}

void EtherealEngineManagers::UpdateManagers(float _dt)
{
	g_DeltaTimeRaw = _dt;
	g_DeltaTime = g_DeltaTimeRaw * g_DeltaTimeFactor;
	//g_DeltaTime = std::min(g_DeltaTime, 0.1f);
	_dt = g_DeltaTime;

	// must be first
	g_RenderMgr->startFrame();

	PROCESS_MGR(InputMgr)
		PROCESS_MGR(TimeMgr)
		PROCESS_MGR(LoadingMgr)
		PROCESS_MGR(SoundMgr)
		PROCESS_MGR(LevelMgr)
		PROCESS_MGR(ItemMgr)
		PROCESS_MGR(GameMgr)
		PROCESS_MGR(EventMgr)
		PROCESS_MGR(CommandMgr)
		PROCESS_MGR(PhysicMgr)
		PROCESS_MGR(EntityMgr)

		PROCESS_MGR(RenderMgr)
		PROCESS_MGR(GuiMgr)
}

void EtherealEngineManagers::DestroyManagers()
{

	END_MGR(InputMgr)
		END_MGR(TimeMgr)
		END_MGR(LoadingMgr)
		END_MGR(SoundMgr)
		END_MGR(LevelMgr)
		END_MGR(ItemMgr)
		END_MGR(GameMgr)
		END_MGR(EventMgr)
		END_MGR(CommandMgr)
		END_MGR(PhysicMgr)
		END_MGR(EntityMgr)
		END_MGR(RenderMgr)
		END_MGR(GuiMgr)
		delete g_LoadingThread;
	delete g_SaveThread;


		for (int ID = (int)m_managers.size() - 1; ID > 0; --ID)
		{
			Manager* manager = m_managers[ID];
			m_managers[ID] = NULL;
			delete manager;
		}

	m_managers.clear();
}

Manager* EtherealEngineManagers::getManager(ManagerType::Enum type)
{
	for (uint32_t ID = 0; ID < m_managers.size(); ++ID)
	{
		if (m_managers[ID]->getType() == type)
			return m_managers[ID];
	}
	return NULL;
}

bool EtherealEngineManagers::isRunning() 
{ 
	return g_GameMgr->isRunning(); 
}