#pragma once
#include <vector>


#define DECLARE_MANAGER(ManagerClass)\
	class ManagerClass; extern ManagerClass* g_##ManagerClass; extern bool g_activate##ManagerClass;

extern float g_DeltaTimeRaw;
extern float g_DeltaTime;
extern float g_DeltaTimeFactor;

DECLARE_MANAGER(TimeMgr)
DECLARE_MANAGER(SoundMgr)
DECLARE_MANAGER(LevelMgr)
DECLARE_MANAGER(ItemMgr)
DECLARE_MANAGER(InputMgr)
DECLARE_MANAGER(GameMgr)
DECLARE_MANAGER(EventMgr)
DECLARE_MANAGER(EngineMgr)
DECLARE_MANAGER(RenderMgr)
DECLARE_MANAGER(EntityMgr)

#define TIME_MGR	g_TimeMgr;
#define SOUND_MGR	g_SoundMgr;
#define LEVEL_MGR	g_LevelMgr;
#define ITEM_MGR	g_ItemMgr;
#define INPUT_MGR	g_InputMgr;
#define GAME_MGR	g_GameMgr;
#define EVENT_MGR	g_EventMgr;
#define ENGINE_MGR	g_EngineMgr;
#define RENDER_MGR	g_RenderMgr;
#define ENTITY_MGR	g_EntityMgr;

class Manager;

class EtherealDreamManagers
{
public:
	static EtherealDreamManagers* getSingleton()
	{
		return m_instance;
	}

	EtherealDreamManagers();

	void					CreateManagers();
	void					InitManagers();
	void					UpdateManagers(float _dt);
	void					DestroyManagers();
	std::vector<Manager*>&	getManagers() { return m_managers; }
	Manager*				getManager(ManagerType _type);



private:
	std::vector<Manager*>			m_managers;
	static EtherealDreamManagers*	m_instance;
};