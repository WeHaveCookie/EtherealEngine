#include "stdafx.h"
#include "EtherealDreamManagers.h"

#include "Manager/Time/TimeMgr.h"
#include "Manager/Sound/SoundMgr.h"
#include "Manager/Level/LevelMgr.h"
#include "Manager/Item/ItemMgr.h"
#include "Manager/Input/InputMgr.h"
#include "Manager/Game/GameMgr.h"
#include "Manager/Event/EventMgr.h"
#include "Manager/Engine/EngineMgr.h"
#include "Manager/Render/RenderMgr.h"

EtherealDreamManagers* EtherealDreamManagers::m_instance = NULL;

#define REGISTER_MANAGER(ManagerClass) ManagerClass* g_##ManagerClass = NULL;

#define CREATE_MGR(ManagerClass) g_##ManagerClass = new ManagerClass(); m_managers.push_back(g_##ManagerClass);
#define INIT_MGR(ManagerClass) g_##ManagerClass->Init();
#define PROCESS_MGR(ManagerClass) g_##ManagerClass->Process(g_DeltaTime);
#define END_MGR(ManagerClass) g_##ManagerClass->End();	


float g_DeltaTimeRaw = 0.33f;
float g_DeltaTime = 0.33f;
float g_DeltaTimeFactor = 1.f;

REGISTER_MANAGER(TimeMgr)
REGISTER_MANAGER(SoundMgr)
REGISTER_MANAGER(LevelMgr)
REGISTER_MANAGER(ItemMgr)
REGISTER_MANAGER(InputMgr)
REGISTER_MANAGER(GameMgr)
REGISTER_MANAGER(EventMgr)
REGISTER_MANAGER(EngineMgr)
REGISTER_MANAGER(RenderMgr)

EtherealDreamManagers::EtherealDreamManagers()
{
	m_instance = this;
}


void EtherealDreamManagers::CreateManagers()
{
	CREATE_MGR(TimeMgr)
		CREATE_MGR(SoundMgr)
		CREATE_MGR(LevelMgr)
		CREATE_MGR(ItemMgr)
		CREATE_MGR(InputMgr)
		CREATE_MGR(GameMgr)
		CREATE_MGR(EventMgr)
		CREATE_MGR(EngineMgr)
		CREATE_MGR(RenderMgr)
}

void EtherealDreamManagers::InitManagers()
{
	INIT_MGR(TimeMgr)
		INIT_MGR(SoundMgr)
		INIT_MGR(LevelMgr)
		INIT_MGR(ItemMgr)
		INIT_MGR(InputMgr)
		INIT_MGR(GameMgr)
		INIT_MGR(EventMgr)
		INIT_MGR(EngineMgr)
		INIT_MGR(RenderMgr)

}

void EtherealDreamManagers::UpdateManagers(float _dt)
{
	g_DeltaTimeRaw = _dt;
	g_DeltaTime = g_DeltaTimeRaw * g_DeltaTimeFactor;
	g_DeltaTime = std::min(g_DeltaTime, 0.1f);
	_dt = g_DeltaTime;

	// must be first
	g_RenderMgr->StartFrame();

	PROCESS_MGR(TimeMgr)
		PROCESS_MGR(SoundMgr)
		PROCESS_MGR(LevelMgr)
		PROCESS_MGR(ItemMgr)
		PROCESS_MGR(InputMgr)
		PROCESS_MGR(GameMgr)
		PROCESS_MGR(EventMgr)
		PROCESS_MGR(EngineMgr)
}

void EtherealDreamManagers::DestroyManagers()
{

	END_MGR(TimeMgr)
		END_MGR(SoundMgr)
		END_MGR(LevelMgr)
		END_MGR(ItemMgr)
		END_MGR(InputMgr)
		END_MGR(GameMgr)
		END_MGR(EventMgr)
		END_MGR(EngineMgr)
		END_MGR(RenderMgr)

		for (int ID = (int)m_managers.size() - 1; ID > 0; --ID)
		{
			Manager* manager = m_managers[ID];
			m_managers[ID] = NULL;
			delete manager;
		}

	m_managers.clear();
}

Manager* EtherealDreamManagers::getManager(ManagerType type)
{
	for (int ID = 0; ID < m_managers.size(); ++ID)
	{
		if (m_managers[ID]->getType() == type)
			return m_managers[ID];
	}
	return NULL;
}