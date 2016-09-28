#pragma once
#include <string>
#include <cassert>
#include <iostream>
#include <thread>
#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "MinerKidOwnedStates.h"
#include "fsm/StateMachine.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Level/LevelMgr.h"
template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//the amount of nuggets a miner can carry
const int MaxNuggetsKid = 3;
//above this value a miner is thirsty
const int ThirstLevelKid = 5;
//above this value a miner is sleepy
const int TirednessThresholdKid = 8;
//above this value a kid is happy
const int HappyLevel = 5;

const int BoredLevel = 5;

struct UpdateTask;

class MinerKid : public BaseGameEntity
{
private:

	//an instance of the state machine class
	static StateMachine<MinerKid>*  m_pStateMachine;

	location_type         m_Location;

	//how many nuggets the miner has in his pockets
	int                   m_iStoneCarried;

	int                   m_iHappy;

	//the higher the value, the thirstier the miner
	static int                   m_iThirst;

	//the higher the value, the more tired the miner
	int                   m_iFatigue;

	bool				m_playWithMom;
	int					m_bored;
	uint32_t			m_entityId;

	struct Task;

	Task pop();
	static uint32_t threadFunc(void* arg);

	class TaskQueue;
	TaskQueue*		 m_taskQueue;
	std::thread		 m_thread;

	bool			 m_running;

public:

	MinerKid(int id);

	~MinerKid() { delete m_pStateMachine; }

	void start();
	void stop();

	void pushTask(int counter);

	//this must be implemented
	static void Update();

	//so must this
	virtual bool  HandleMessage(const Telegram& msg);


	StateMachine<MinerKid>* GetFSM()const { EntityMgr::getSingleton()->getEntity(m_entityId)->setText(""); return m_pStateMachine; }



	//-------------------------------------------------------------accessors
	location_type Location()const { return m_Location; }
	void          ChangeLocation(location_type loc) 
	{ 
		m_Location = loc; 
		EntityMgr::getSingleton()->getEntity(m_entityId)->setPosition(LevelMgr::getSingleton()->getPosLocation(m_Location) + Vector2(0.0f, 100.0f));
	}

	int           StoneCarried()const { return m_iStoneCarried; }
	void          SetStoneCarried(int val) { m_iStoneCarried = val; }
	void          AddToStoneCarried(int val);
	bool          PocketsFull()const { return m_iStoneCarried >= MaxNuggetsKid; }

	bool          Fatigued()const;
	void          DecreaseFatigue() { m_iFatigue -= 2; }
	void          IncreaseFatigue() { m_iFatigue += 1; }

	bool          Thirsty()const;
	void			Cry() const;

	void		setPlaying(bool b) { m_playWithMom = b; }

	bool			Happy() const;
	void			addHappyness(int i);
	bool			isPlaying() { return m_playWithMom; }

	void			IncreaseBored() { m_bored++; }
	void			setBored(int i) { m_bored = i; }
	void			PlayWithPotion();
	bool			BoredToWait();
	void			ResetThristy() { m_iThirst = 0; }

	void	displayText(const char* txt)
	{
		EntityMgr::getSingleton()->getEntity(m_entityId)->setText(txt);
	}

	void displayEnterText(const char* txt)
	{
		EntityMgr::getSingleton()->getEntity(m_entityId)->setEnterText(txt);
	}

	void displayExitText(const char* txt)
	{
		EntityMgr::getSingleton()->getEntity(m_entityId)->setExitText(txt);
	}

	void displayOnMessageText(const char* txt)
	{
		EntityMgr::getSingleton()->getEntity(m_entityId)->setOnMessage(txt);
	}
};