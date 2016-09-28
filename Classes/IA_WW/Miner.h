#ifndef MINER_H
#define MINER_H
//------------------------------------------------------------------------
//
//  Name:   Miner.h
//
//  Desc:   A class defining a goldminer.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>
#include <thread>
#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "MinerOwnedStates.h"
#include "fsm/StateMachine.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Level/LevelMgr.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//the amount of gold a miner must have before he feels he can go home
const int ComfortLevel       = 5;
//the amount of nuggets a miner can carry
const int MaxNuggets         = 3;
//above this value a miner is thirsty
const int ThirstLevel        = 5;
//above this value a miner is sleepy
const int TirednessThreshold = 5;

struct UpdateTask;

class Miner : public BaseGameEntity
{
private:

  //an instance of the state machine class
  static StateMachine<Miner>*  m_pStateMachineMiner;
  
  location_type         m_Location;

  //how many nuggets the miner has in his pockets
  int                   m_iGoldCarried;

  int                   m_iMoneyInBank;

  //the higher the value, the thirstier the miner
  static int                   m_iThirst;

  //the higher the value, the more tired the miner
  int                   m_iFatigue;

  uint32_t				m_entityId;

  struct Task;

  Task pop();
  static uint32_t threadFunc(void* arg);

  class TaskQueue;
  TaskQueue*		 m_taskQueue;
  std::thread		 m_thread;

  bool			 m_running;

public:

	Miner(int id);

  ~Miner(){delete m_pStateMachineMiner;}

  void start();
  void stop();

  void pushTask(int counter);

  //this must be implemented
  static void Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  
  StateMachine<Miner>* GetFSM()const{ EntityMgr::getSingleton()->getEntity(m_entityId)->setText(""); return m_pStateMachineMiner;}


  
  //-------------------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc)
  {
	  m_Location=loc;
	  EntityMgr::getSingleton()->getEntity(m_entityId)->setPosition(LevelMgr::getSingleton()->getPosLocation(m_Location) - Vector2(0.0f, 100.0f));
  }
    
  int           GoldCarried()const{return m_iGoldCarried;}
  void          SetGoldCarried(int val){m_iGoldCarried = val;}
  void          AddToGoldCarried(int val);
  bool          PocketsFull()const{return m_iGoldCarried >= MaxNuggets;}

  bool          Fatigued()const;
  void          DecreaseFatigue(){m_iFatigue -= 1;}
  void          IncreaseFatigue(){m_iFatigue += 1;}

  int           Wealth()const{return m_iMoneyInBank;}
  void          SetWealth(int val){m_iMoneyInBank = val;}
  void          AddToWealth(int val);

  bool          Thirsty()const; 
  void          BuyAndDrinkAWhiskey(){m_iThirst = 0; m_iMoneyInBank-=2;}

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



#endif
