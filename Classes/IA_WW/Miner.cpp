#include "stdafx.h"
#include "Miner.h"
#include "blockingconcurrentqueue.h"
#include "Manager/MinerMgr.h"

StateMachine<Miner>*  Miner::m_pStateMachineMiner = NULL;
int Miner::m_iThirst = 0;

struct Miner::Task
{
	int m_counter;
};

class Miner::TaskQueue : public moodycamel::BlockingConcurrentQueue<Task> {};

Miner::Miner(int id) 
	:m_Location(shack),
	m_iGoldCarried(0),
	m_iMoneyInBank(0),
	m_iFatigue(0),
	BaseGameEntity(id),
	m_running(false)
{
	m_taskQueue = new TaskQueue();

	m_iThirst = 0;
	//set up state machine
	m_pStateMachineMiner = new StateMachine<Miner>(this);

	m_pStateMachineMiner->SetCurrentState(GoHomeAndSleepTilRested::Instance());

	m_entityId = EntityMgr::getSingleton()->createEntity("Data/Character/miner.json")->getUID();
	start();

	/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
}

bool Miner::HandleMessage(const Telegram& msg)
{
  return m_pStateMachineMiner->HandleMessage(msg);
}


void Miner::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);

  m_iThirst += 1;
  
  m_pStateMachineMiner->Update();
}



void Miner::AddToGoldCarried(const int val)
{
  m_iGoldCarried += val;

  if (m_iGoldCarried < 0) m_iGoldCarried = 0;
}

void Miner::AddToWealth(const int val)
{
  m_iMoneyInBank += val;

  if (m_iMoneyInBank < 0) m_iMoneyInBank = 0;
}

bool Miner::Thirsty()const
{
  if (m_iThirst >= ThirstLevel){return true;}

  return false;
}

bool Miner::Fatigued()const
{
  if (m_iFatigue > TirednessThreshold)
  {
    return true;
  }

  return false;
}

void Miner::start()
{
	if (m_running)
		return;

	m_running = true;

	m_thread = std::thread(threadFunc, this);
}

void Miner::stop()
{
	if (!m_running)
		return;

	m_running = false;
	Task task; // Dummy task for exit thread
	task.m_counter = 0;
	m_taskQueue->enqueue(task);
	m_thread.join();
}

void Miner::pushTask(int counter)
{
	Task task;
	task.m_counter = counter;

	m_taskQueue->enqueue(task);
}

Miner::Task Miner::pop()
{
	Task task;
	m_taskQueue->wait_dequeue(task);
	return task;
}

uint32_t Miner::threadFunc(void* arg)
{
	Miner* loadingThread = (Miner*)arg;

	while (loadingThread->m_running)
	{
		Task task = loadingThread->pop();

		for (int i = 0; i < task.m_counter; i++)
		{
			Update();
		}
	}

	return 0;
}