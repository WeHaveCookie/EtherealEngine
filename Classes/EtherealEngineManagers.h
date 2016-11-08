#pragma once
#include <vector>
#include "Manager/Manager.h"
#include "Thread/Thread.h"

#define DECLARE_MANAGER(ManagerClass)\
	class ManagerClass; extern ManagerClass* g_##ManagerClass; extern bool g_activate##ManagerClass;

extern float g_DeltaTimeRaw;
extern float g_DeltaTime;
extern float g_DeltaTimeFactor;
extern float g_Framerate;

DECLARE_MANAGER(TimeMgr)
DECLARE_MANAGER(LoadingMgr)
DECLARE_MANAGER(SoundMgr)
DECLARE_MANAGER(LevelMgr)
DECLARE_MANAGER(ItemMgr)
DECLARE_MANAGER(InputMgr)
DECLARE_MANAGER(GameMgr)
DECLARE_MANAGER(EventMgr)
DECLARE_MANAGER(PhysicMgr)
DECLARE_MANAGER(RenderMgr)
DECLARE_MANAGER(EntityMgr)
DECLARE_MANAGER(FileMgr)
DECLARE_MANAGER(PersistentMgr)
DECLARE_MANAGER(GuiMgr)
DECLARE_MANAGER(CommandMgr)

DECLARE_MANAGER(LoadingThread)
DECLARE_MANAGER(SaveThread)

#define TIME_MGR		g_TimeMgr;
#define LOADING_MGR		g_LoadingMgr;
#define SOUND_MGR		g_SoundMgr;
#define LEVEL_MGR		g_LevelMgr;
#define ITEM_MGR		g_ItemMgr;
#define INPUT_MGR		g_InputMgr;
#define GAME_MGR		g_GameMgr;
#define EVENT_MGR		g_EventMgr;
#define PHYSIC_MGR		g_PhysicMgr;
#define RENDER_MGR		g_RenderMgr;
#define ENTITY_MGR		g_EntityMgr;
#define FILE_MGR		g_FileMgr;
#define PERSISTENT_MGR	g_PersistentMgr;
#define GUI_MGR			g_GuiMgr;
#define COMMAND_MGR		g_CommandMgr;


#define LOAD_THREAD	g_LoadingThread;
#define SAVE_THREAD g_SaveThread;

class Manager;

class EtherealEngineManagers
{
public:
	static EtherealEngineManagers* getSingleton()
	{
		return m_instance;
	}

	EtherealEngineManagers();

	void					CreateManagers();
	void					InitManagers();
	void					UpdateManagers(float _dt);
	void					DestroyManagers();
	std::vector<Manager*>&	getManagers() { return m_managers; }
	Manager*				getManager(ManagerType::Enum _type);
	bool					isRunning();


private:
	std::vector<Manager*>			m_managers;
	static EtherealEngineManagers*	m_instance;
};