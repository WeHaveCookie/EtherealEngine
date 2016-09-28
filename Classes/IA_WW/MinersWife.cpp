#include "stdafx.h"
#include "MinersWife.h"
#include "blockingconcurrentqueue.h"
#include "Manager/MinerMgr.h"

StateMachine<MinersWife>* MinersWife::m_pStateMachine = NULL;

struct MinersWife::Task
{
	int m_counter;
};

class MinersWife::TaskQueue : public moodycamel::BlockingConcurrentQueue<Task> {};

MinersWife::MinersWife(int id)
	:m_Location(shack),
	m_bCooking(false),
	BaseGameEntity(id),
	m_running(false)

{
	m_taskQueue = new TaskQueue();
	//set up the state machine
	m_pStateMachine = new StateMachine<MinersWife>(this);

	m_pStateMachine->SetCurrentState(DoHouseWork::Instance());

	m_pStateMachine->SetGlobalState(WifesGlobalState::Instance());

	m_entityId = EntityMgr::getSingleton()->createEntity("Data/Character/minerWife.json")->getUID();
	start();
}

bool MinersWife::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void MinersWife::Update()
{
  //set text color to green
  SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
 
  m_pStateMachine->Update();
}

void MinersWife::start()
{
	if (m_running)
		return;

	m_running = true;

	m_thread = std::thread(threadFunc, this);
}

void MinersWife::stop()
{
	if (!m_running)
		return;

	m_running = false;
	Task task; // Dummy task for exit thread
	task.m_counter = 0;
	m_taskQueue->enqueue(task);
	m_thread.join();
}

void MinersWife::pushTask(int counter)
{
	Task task;
	task.m_counter = counter;

	m_taskQueue->enqueue(task);
}

MinersWife::Task MinersWife::pop()
{
	Task task;
	m_taskQueue->wait_dequeue(task);
	return task;
}

uint32_t MinersWife::threadFunc(void* arg)
{
	MinersWife* loadingThread = (MinersWife*)arg;

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