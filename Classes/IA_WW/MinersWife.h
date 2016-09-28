#ifndef MINERSWIFE_H
#define MINERSWIFE_H
//------------------------------------------------------------------------
//
//  Name: MinersWife.h
//
//  Desc: class to implement Miner Bob's wife.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "MinersWifeOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "Miner.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"
#include "Manager/Entity/EntityMgr.h"
#include "Manager/Level/LevelMgr.h"
#include <thread>

struct UpdateTask;

class MinersWife : public BaseGameEntity
{
private:

  //an instance of the state machine class
  static StateMachine<MinersWife>* m_pStateMachine;

  location_type   m_Location;

  //is she presently cooking?
  bool            m_bCooking;

  uint32_t		m_entityId;

  struct Task;

  Task pop();
  static uint32_t threadFunc(void* arg);

  class TaskQueue;
  TaskQueue*		 m_taskQueue;
  std::thread		 m_thread;

  bool			 m_running;

public:

	MinersWife(int id);

  ~MinersWife(){delete m_pStateMachine;}



  void start();
  void stop();

  void pushTask(int counter);

  //this must be implemented
  static void          Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  StateMachine<MinersWife>* GetFSM()const{ EntityMgr::getSingleton()->getEntity(m_entityId)->setText(""); return m_pStateMachine;}

  //----------------------------------------------------accessors
  location_type Location()const{return m_Location;}
  void          ChangeLocation(location_type loc)
  {
	  m_Location=loc;
	  EntityMgr::getSingleton()->getEntity(m_entityId)->setPosition(LevelMgr::getSingleton()->getPosLocation(m_Location));
  }

  bool          Cooking()const{return m_bCooking;}
  void          SetCooking(bool val){m_bCooking = val;}

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
