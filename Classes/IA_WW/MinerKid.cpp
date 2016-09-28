#include "stdafx.h"
#include "MinerKid.h"
#include "blockingconcurrentqueue.h"
#include "Manager/MinerMgr.h"

StateMachine<MinerKid>*  MinerKid::m_pStateMachine = NULL;
int MinerKid::m_iThirst = 0;

struct MinerKid::Task
{
	int m_counter;
};

class MinerKid::TaskQueue : public moodycamel::BlockingConcurrentQueue<Task> {};

MinerKid::MinerKid(int id)
	:m_Location(shack),
	m_iStoneCarried(0),
	m_iHappy(0),
	m_iFatigue(0),
	m_bored(0),
	m_playWithMom(false),
	BaseGameEntity(id),
	m_running(false)

{
	m_taskQueue = new TaskQueue();
	m_iThirst = 0;
	//set up state machine
	m_pStateMachine = new StateMachine<MinerKid>(this);

	m_pStateMachine->SetCurrentState(GoHomeAndSleep::Instance());

	m_entityId = EntityMgr::getSingleton()->createEntity("Data/Character/minerKid.json")->getUID();

	/* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
	start();
}

bool MinerKid::HandleMessage(const Telegram& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}


void MinerKid::Update()
{
	SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);

	m_iThirst += 1;

	m_pStateMachine->Update();
}

void MinerKid::AddToStoneCarried(const int val)
{
	m_iStoneCarried += val;

	if (m_iStoneCarried < 0) m_iStoneCarried = 0;
}

bool MinerKid::Thirsty()const
{
	if (m_iThirst >= ThirstLevelKid) { return true; }

	return false;
}

bool MinerKid::Fatigued()const
{
	if (m_iFatigue > TirednessThresholdKid)
	{
		return true;
	}

	return false;
}

void MinerKid::Cry() const
{

}

bool MinerKid::Happy() const
{
	return m_iHappy >= HappyLevel;
}

void MinerKid::addHappyness(int i) 
{
	m_iHappy += i;
}

void MinerKid::PlayVideoGame()
{
	m_iThirst = 0;
}

bool MinerKid::BoredToWait()
{
	return m_bored >= BoredLevel;
}


void MinerKid::start()
{
	if (m_running)
		return;

	m_running = true;

	m_thread = std::thread(threadFunc, this);
}

void MinerKid::stop()
{
	if (!m_running)
		return;

	m_running = false;
	Task task; // Dummy task for exit thread
	task.m_counter = 0;
	m_taskQueue->enqueue(task);
	m_thread.join();
}

void MinerKid::pushTask(int counter)
{
	Task task;
	task.m_counter = counter;

	m_taskQueue->enqueue(task);
}

MinerKid::Task MinerKid::pop()
{
	Task task;
	m_taskQueue->wait_dequeue(task);
	return task;
}

uint32_t MinerKid::threadFunc(void* arg)
{
	MinerKid* loadingThread = (MinerKid*)arg;

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
